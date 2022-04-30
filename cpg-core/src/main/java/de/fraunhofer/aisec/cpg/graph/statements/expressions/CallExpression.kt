/*
 * Copyright (c) 2020, Fraunhofer AISEC. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *                    $$$$$$\  $$$$$$$\   $$$$$$\
 *                   $$  __$$\ $$  __$$\ $$  __$$\
 *                   $$ /  \__|$$ |  $$ |$$ /  \__|
 *                   $$ |      $$$$$$$  |$$ |$$$$\
 *                   $$ |      $$  ____/ $$ |\_$$ |
 *                   $$ |  $$\ $$ |      $$ |  $$ |
 *                   \$$$$$   |$$ |      \$$$$$   |
 *                    \______/ \__|       \______/
 *
 */
package de.fraunhofer.aisec.cpg.graph.statements.expressions

import de.fraunhofer.aisec.cpg.graph.*
import de.fraunhofer.aisec.cpg.graph.HasType.SecondaryTypeEdge
import de.fraunhofer.aisec.cpg.graph.declarations.Declaration
import de.fraunhofer.aisec.cpg.graph.declarations.FunctionDeclaration
import de.fraunhofer.aisec.cpg.graph.declarations.TemplateDeclaration
import de.fraunhofer.aisec.cpg.graph.declarations.TemplateDeclaration.TemplateInitialization
import de.fraunhofer.aisec.cpg.graph.edge.Properties
import de.fraunhofer.aisec.cpg.graph.edge.PropertyEdge
import de.fraunhofer.aisec.cpg.graph.edge.PropertyEdge.Companion.propertyEqualsList
import de.fraunhofer.aisec.cpg.graph.edge.PropertyEdge.Companion.transformIntoOutgoingPropertyEdgeList
import de.fraunhofer.aisec.cpg.graph.edge.PropertyEdge.Companion.unwrap
import de.fraunhofer.aisec.cpg.graph.types.Type
import de.fraunhofer.aisec.cpg.helpers.Util
import de.fraunhofer.aisec.cpg.passes.CallResolution
import de.fraunhofer.aisec.cpg.passes.Inference
import java.util.*
import kotlin.collections.ArrayList
import org.apache.commons.lang3.builder.ToStringBuilder
import org.neo4j.ogm.annotation.Relationship

/**
 * An expression, which calls an expression (the [callee]) with a list of arguments. The callee is
 * usually a reference to a function (using a [DeclaredReferenceExpression]) or method (using a
 * [MemberExpression]), but depending on the languages, other expressions also might be "callable".
 *
 * After call resolution, this expression connected via the `INVOKES` edge (see [invokesEdges]) to
 * its [FunctionDeclaration].
 */
open class CallExpression(callee: Expression? = null) :
    Expression(),
    HasType.TypeListener,
    SecondaryTypeEdge,
    ResolutionDecider<CallExpression, DeclaredReferenceExpression> {

    /**
     * The expression we are trying to "call". In most cases this is a [DeclaredReferenceExpression]
     * to a function or a [MemberExpression] for a member call. However, there might be other
     * expression one is trying to call, depending on the language.
     *
     * For now, this is nullable, because of [ConstructExpression] does not have a callee, but
     * depends on [CallExpression].
     */
    @field:SubGraph("AST")
    var callee: Expression? = null
        set(value) {
            // We also want to update this node's name, based on the callee. This is purely for
            // readability reasons
            field = value
            this.name = value?.name ?: ""

            // Register this call as the resolution decider
            val needsResolution = callee as? NeedsResolution<DeclaredReferenceExpression>
            needsResolution?.resolutionDecider = this
        }

    init {
        this.callee = callee
    }

    /** Connection to its [FunctionDeclaration]. This will be populated by the [CallResolver]. */
    @field:Relationship(value = "INVOKES", direction = "OUTGOING")
    var invokesEdges: List<PropertyEdge<FunctionDeclaration>>
        protected set

    var invokes: List<FunctionDeclaration>
        get(): List<FunctionDeclaration> {
            val targets: MutableList<FunctionDeclaration> = ArrayList()
            for (propertyEdge in invokesEdges) {
                targets.add(propertyEdge.end)
            }
            return Collections.unmodifiableList(targets)
        }
        set(value) {
            unwrap(invokesEdges).forEach {
                it.unregisterTypeListener(this)
                Util.detachCallParameters(it, arguments)
                removePrevDFG(it)
            }
            invokesEdges = transformIntoOutgoingPropertyEdgeList(value, this)
            value.forEach {
                it.registerTypeListener(this)
                Util.attachCallParameters(it, arguments)
                addPrevDFG(it)
            }
        }

    // only there to not break too many things at once
    @Deprecated(message = "we need to redesign the whole naming system") var fqn: String? = ""

    /** The list of arguments. */
    @field:Relationship(value = "ARGUMENTS", direction = "OUTGOING")
    @field:SubGraph("AST")
    var argumentEdges: MutableList<PropertyEdge<Expression>>

    var arguments: List<Expression>
        get() {
            val targets: MutableList<Expression> = ArrayList()
            for (propertyEdge in argumentEdges) {
                targets.add(propertyEdge.end)
            }
            return Collections.unmodifiableList(targets)
        }
        set(value) {
            this.argumentEdges = transformIntoOutgoingPropertyEdgeList(value, this)
        }

    fun setArgument(index: Int, argument: Expression) {
        argumentEdges[index].end = argument
    }

    @JvmOverloads
    fun addArgument(expression: Expression, name: String? = null) {
        val propertyEdge = PropertyEdge(this, expression)
        propertyEdge.addProperty(Properties.INDEX, argumentEdges.size)
        if (name != null) {
            propertyEdge.addProperty(Properties.NAME, name)
        }
        argumentEdges.add(propertyEdge)
    }

    val signature: List<Type>
        get() = arguments.map { it.getType() }

    var template = false
        set(value) {
            field = value
            if (value) {
                templateParametersEdge = ArrayList()
            }
        }

    /** If the CallExpression instantiates a Template, the call can provide template parameters */
    @Relationship(value = "TEMPLATE_PARAMETERS", direction = "OUTGOING")
    @SubGraph("AST")
    var templateParametersEdge: MutableList<PropertyEdge<Node>>? = null

    val templateParameters: List<Node>?
        get() {
            return templateParametersEdge?.let { unwrap(it) }
        }

    /**
     * If the CallExpression instantiates a Template the CallExpression is connected to the template
     * which is instantiated. This is required by the expansion pass to access the Template
     * directly. The invokes edge will still point to the realization of the template.
     */
    @Relationship(value = "TEMPLATE_INSTANTIATION", direction = "OUTGOING")
    var templateInstantiation: TemplateDeclaration? = null
        set(value) {
            field = value
            template = field != null
        }

    val typeTemplateParameters: List<Type>?
        get() {
            if (templateParametersEdge == null) {
                return null
            }
            val types: MutableList<Type> = ArrayList()
            for (n in templateParametersEdge!!) {
                if (n is Type) {
                    types.add(n)
                }
            }
            return types
        }

    fun addTemplateParameter(
        typeTemplateParam: Type,
        templateInitialization: TemplateInitialization?
    ) {
        if (templateParametersEdge == null) {
            templateParametersEdge = ArrayList()
        }
        val propertyEdge = PropertyEdge<Node>(this, typeTemplateParam)
        propertyEdge.addProperty(Properties.INDEX, templateParametersEdge!!.size)
        propertyEdge.addProperty(Properties.INSTANTIATION, templateInitialization)
        templateParametersEdge!!.add(propertyEdge)
        template = true
    }

    fun replaceTypeTemplateParameter(oldType: Type?, newType: Type) {
        if (templateParametersEdge == null) {
            return
        }
        for (i in templateParametersEdge!!.indices) {
            val propertyEdge = templateParametersEdge!![i]
            if (propertyEdge.end == oldType) {
                propertyEdge.end = newType
            }
        }
    }

    fun addTemplateParameter(
        expressionTemplateParam: Expression,
        templateInitialization: TemplateInitialization?
    ) {
        if (templateParametersEdge == null) {
            templateParametersEdge = ArrayList()
        }
        val propertyEdge = PropertyEdge<Node>(this, expressionTemplateParam)
        propertyEdge.addProperty(Properties.INDEX, templateParametersEdge!!.size)
        propertyEdge.addProperty(Properties.INSTANTIATION, templateInitialization)
        templateParametersEdge!!.add(propertyEdge)
        template = true
    }

    fun addTemplateParameter(
        templateParam: Node?,
        templateInitialization: TemplateInitialization?
    ) {
        if (templateParam is Expression) {
            addTemplateParameter(templateParam, templateInitialization)
        } else if (templateParam is Type) {
            addTemplateParameter(templateParam, templateInitialization)
        }
    }

    fun addExplicitTemplateParameter(templateParameter: Node?) {
        addTemplateParameter(templateParameter, TemplateInitialization.EXPLICIT)
    }

    fun addExplicitTemplateParameters(templateParameters: List<Node?>) {
        for (node in templateParameters) {
            addTemplateParameter(node, TemplateInitialization.EXPLICIT)
        }
    }

    fun removeRealization(templateParam: Node?) {
        if (templateParametersEdge == null) {
            return
        }
        templateParametersEdge!!.removeIf { propertyEdge: PropertyEdge<Node>? ->
            propertyEdge!!.end.equals(templateParam)
        }
    }

    fun updateTemplateParameters(
        initializationType: Map<Node?, TemplateInitialization?>,
        orderedInitializationSignature: List<Node>
    ) {
        if (templateParametersEdge == null) {
            return
        }
        for (edge in templateParametersEdge!!) {
            if (edge.getProperty(Properties.INSTANTIATION) != null &&
                    (edge.getProperty(Properties.INSTANTIATION) ==
                        TemplateInitialization.UNKNOWN) &&
                    initializationType.containsKey(edge.end)
            ) {
                edge.addProperty(Properties.INSTANTIATION, initializationType[edge.end])
            }
        }
        for (i in templateParametersEdge!!.size until orderedInitializationSignature.size) {
            val propertyEdge = PropertyEdge(this, orderedInitializationSignature[i])
            propertyEdge.addProperty(Properties.INDEX, templateParametersEdge!!.size)
            propertyEdge.addProperty(
                Properties.INSTANTIATION,
                initializationType.getOrDefault(
                    orderedInitializationSignature[i],
                    TemplateInitialization.UNKNOWN
                )
            )
            templateParametersEdge!!.add(propertyEdge)
        }
    }

    fun instantiatesTemplate(): Boolean {
        return templateInstantiation != null || templateParametersEdge != null || template
    }

    override fun typeChanged(src: HasType, root: List<HasType>, oldType: Type) {
        if (!TypeManager.isTypeSystemActive()) {
            return
        }

        val previous = type
        val types = invokesEdges.map { it.end }.map { it.type }.filter { Objects.nonNull(it) }
        val alternative = if (types.isNotEmpty()) types[0] else null
        val commonType = TypeManager.getInstance().getCommonType(types).orElse(alternative)
        val subTypes: MutableList<Type> = ArrayList(possibleSubTypes)
        subTypes.remove(oldType)
        subTypes.addAll(types)
        setType(commonType, root)
        setPossibleSubTypes(subTypes, root)
        if (previous != type) {
            type.typeOrigin = Type.Origin.DATAFLOW
        }
    }

    override fun possibleSubTypesChanged(
        src: HasType,
        root: List<HasType>,
    ) {
        if (!TypeManager.isTypeSystemActive()) {
            return
        }
        val subTypes: MutableList<Type> = ArrayList(possibleSubTypes)
        subTypes.addAll(src.possibleSubTypes)
        setPossibleSubTypes(subTypes, root)
    }

    override fun toString(): String {
        return ToStringBuilder(this, TO_STRING_STYLE).appendSuper(super.toString()).toString()
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) {
            return true
        }
        if (other !is CallExpression) {
            return false
        }
        return (super.equals(other) &&
            arguments == other.arguments &&
            propertyEqualsList(argumentEdges, other.argumentEdges) &&
            invokes == other.invokes &&
            propertyEqualsList(invokesEdges, other.invokesEdges) &&
            (templateParameters == other.templateParameters ||
                templateParameters == other.templateParameters &&
                    propertyEqualsList(templateParametersEdge!!, other.templateParametersEdge!!)) &&
            (templateInstantiation == other.templateInstantiation ||
                templateInstantiation == other.templateInstantiation) &&
            template == other.template)
    }

    override fun hashCode(): Int {
        return super.hashCode()
    }

    override fun updateType(typeState: Collection<Type>) {
        if (templateParametersEdge == null) {
            return
        }
        for (t in typeTemplateParameters!!) {
            for (t2 in typeState) {
                if (t2 == t) {
                    replaceTypeTemplateParameter(t, t2)
                }
            }
        }
    }

    init {
        this.invokesEdges = mutableListOf()
        this.argumentEdges = mutableListOf()
    }

    override fun decide(
        symbols: List<Declaration>,
        source: DeclaredReferenceExpression,
        inference: Inference?
    ): Declaration? {
        return CallResolution.decide(this, source, symbols, inference)
    }
}
