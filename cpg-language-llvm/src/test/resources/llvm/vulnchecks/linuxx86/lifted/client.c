//
// This file was generated by the Retargetable Decompiler
// Website: https://retdec.com
// Copyright (c) Retargetable Decompiler <info@retdec.com>
//

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

// ------------------------ Structures ------------------------

struct sockaddr {
    int64_t e0;
    char e1[14];
};

// ------------------- Function Prototypes --------------------

int64_t _Z14initTLSContextv(int64_t a1, int64_t a2, int64_t a3);
int64_t _Z22failSetInsecureCiphersP10ssl_ctx_st(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
int64_t _Z23failDisableVerificationP10ssl_ctx_st(int64_t * a1);
int64_t _Z25failSetInsecureCiphersSTLP10ssl_ctx_st(int64_t * a1);
int64_t _Z28failSetInsecureCiphersGlobalP10ssl_ctx_st(int64_t * a1);
int64_t _Z29failSetInsecureCiphersLiteralP10ssl_ctx_st(int64_t * a1);
int64_t _Z9connectToNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEi(int64_t * result, int64_t a2, int32_t a3);
int64_t function_401050(int64_t a1, int32_t a2);
int64_t function_401070(int64_t a1);
int64_t function_4010a0(int64_t a1, int64_t a2, int64_t a3);
int64_t function_4010c0(void);
int64_t function_4010e0(int64_t a1);
int64_t function_4010f0(void);
int64_t function_401170(int64_t a1);
int64_t function_401180(int64_t a1);
int64_t function_401190(int64_t a1, int64_t * a2);
int64_t function_4011c0(int64_t a1);
int64_t function_4011e0(int64_t a1);
int64_t function_401210(int64_t a1, int64_t a2, int64_t a3);
int64_t function_401220(int32_t a1, int64_t a2);

// --------------------- Global Variables ---------------------

int64_t g1 = 0; // 0x404140
int64_t g2 = 0; // 0x404260
int64_t g3 = 0; // 0x404380

// ------------------------ Functions -------------------------

// Address range: 0x401050 - 0x401056
int64_t function_401050(int64_t a1, int32_t a2) {
    // 0x401050
    return SSL_set_fd();
}

// Address range: 0x401070 - 0x401076
int64_t function_401070(int64_t a1) {
    // 0x401070
    return SSL_new();
}

// Address range: 0x4010a0 - 0x4010a6
int64_t function_4010a0(int64_t a1, int64_t a2, int64_t a3) {
    // 0x4010a0
    return OPENSSL_init_ssl();
}

// Address range: 0x4010c0 - 0x4010c6
int64_t function_4010c0(void) {
    // 0x4010c0
    return ERR_get_error();
}

// Address range: 0x4010e0 - 0x4010e6
int64_t function_4010e0(int64_t a1) {
    // 0x4010e0
    return SSL_CIPHER_get_name();
}

// Address range: 0x4010f0 - 0x4010f6
int64_t function_4010f0(void) {
    // 0x4010f0
    return TLSv1_2_client_method();
}

// Address range: 0x401170 - 0x401176
int64_t function_401170(int64_t a1) {
    // 0x401170
    return SSL_CTX_new();
}

// Address range: 0x401180 - 0x401186
int64_t function_401180(int64_t a1) {
    // 0x401180
    return SSL_connect();
}

// Address range: 0x401190 - 0x401196
int64_t function_401190(int64_t a1, int64_t * a2) {
    // 0x401190
    return SSL_CTX_set_cipher_list();
}

// Address range: 0x4011c0 - 0x4011c6
int64_t function_4011c0(int64_t a1) {
    // 0x4011c0
    return SSL_get_current_cipher();
}

// Address range: 0x4011e0 - 0x4011e6
int64_t function_4011e0(int64_t a1) {
    // 0x4011e0
    return SSL_get_verify_result();
}

// Address range: 0x401210 - 0x401216
int64_t function_401210(int64_t a1, int64_t a2, int64_t a3) {
    // 0x401210
    return SSL_CTX_set_verify();
}

// Address range: 0x401220 - 0x401226
int64_t function_401220(int32_t a1, int64_t a2) {
    // 0x401220
    return ERR_error_string();
}

// Address range: 0x401380 - 0x4014be
// Demangled:     connectTo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int)
int64_t _Z9connectToNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEi(int64_t * result, int64_t a2, int32_t a3) {
    uint32_t sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP); // 0x40139d
    if (sock_fd == 0) {
        // 0x4013af
        printf("Error creating socket.\n");
        // 0x4014b5
        return 0xffffffff;
    }
    int64_t v1 = _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(&g2, "Connecting to "); // 0x4013e0
    int64_t v2 = _ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE((int64_t *)v1, result); // 0x4013ec
    _ZNSolsEPFRSoS_E((int64_t * (*)(int64_t *))_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc((int64_t *)v2, "..."));
    int64_t addr; // bp-40, 0x401380
    memset(&addr, 0, 16);
    addr = 2;
    inet_addr((char *)_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv());
    htons(443);
    int64_t result2; // 0x401380
    if (connect(sock_fd, (struct sockaddr *)&addr, 16) == 0) {
        // 0x4014af
        result2 = sock_fd;
    } else {
        int64_t v3 = _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(&g2, "Error connecting to server."); // 0x40148c
        _ZNSolsEPFRSoS_E((int64_t * (*)(int64_t *))v3);
        result2 = 0xffffffff;
    }
    // 0x4014b5
    return result2;
}

// Address range: 0x4014c0 - 0x4014ea
// Demangled:     failDisableVerification(ssl_ctx_st*)
int64_t _Z23failDisableVerificationP10ssl_ctx_st(int64_t * a1) {
    // 0x4014c0
    return function_401210((int64_t)a1, 1, 0x4014f0);
}

// Address range: 0x401510 - 0x401545
// Demangled:     failSetInsecureCiphers(ssl_ctx_st*)
int64_t _Z22failSetInsecureCiphersP10ssl_ctx_st(int64_t a1, int64_t a2, int64_t a3, int64_t a4) {
    int64_t v1 = 0x484441213a4c4c41; // bp-25, 0x401528
    return function_401190(a1, &v1);
}

// Address range: 0x401550 - 0x401575
// Demangled:     failSetInsecureCiphersLiteral(ssl_ctx_st*)
int64_t _Z29failSetInsecureCiphersLiteralP10ssl_ctx_st(int64_t * a1) {
    // 0x401550
    return function_401190((int64_t)a1, (int64_t *)"ALL:!ADH");
}

// Address range: 0x401580 - 0x4015f0
// Demangled:     failSetInsecureCiphersSTL(ssl_ctx_st*)
int64_t _Z25failSetInsecureCiphersSTLP10ssl_ctx_st(int64_t * a1) {
    // 0x401580
    _ZNSaIcEC1Ev();
    int64_t v1; // bp-48, 0x401580
    _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_((char *)&v1, (int64_t *)"ALL:!ADH");
    _ZNSaIcED1Ev();
    function_401190((int64_t)a1, (int64_t *)_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv());
    return _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev();
}

// Address range: 0x401620 - 0x401645
// Demangled:     failSetInsecureCiphersGlobal(ssl_ctx_st*)
int64_t _Z28failSetInsecureCiphersGlobalP10ssl_ctx_st(int64_t * a1) {
    // 0x401620
    return function_401190((int64_t)a1, (int64_t *)"MD5");
}

// Address range: 0x401660 - 0x4016e3
// Demangled:     initTLSContext()
int64_t _Z14initTLSContextv(int64_t a1, int64_t a2, int64_t a3) {
    // 0x401660
    function_4010a0(0, 0, a3);
    function_4010a0(0x200002, 0, 0);
    int64_t result = function_401170(function_4010f0()); // 0x401693
    _Z22failSetInsecureCiphersP10ssl_ctx_st(result, 0, 0, 0);
    int64_t * v1 = (int64_t *)result; // 0x4016a9
    _Z29failSetInsecureCiphersLiteralP10ssl_ctx_st(v1);
    _Z25failSetInsecureCiphersSTLP10ssl_ctx_st(v1);
    _Z28failSetInsecureCiphersGlobalP10ssl_ctx_st(v1);
    function_401210(result, 1, 0);
    _Z23failDisableVerificationP10ssl_ctx_st(v1);
    return result;
}

// Address range: 0x4016f0 - 0x4019a0
int main(int argc, char ** argv) {
    // 0x4016f0
    int64_t v1; // bp-72, 0x4016f0
    int64_t v2 = &v1; // 0x401710
    _ZNSaIcEC1Ev();
    int64_t v3; // bp-64, 0x4016f0
    _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_((char *)&v3, (int64_t *)"172.217.18.99");
    int64_t v4 = _Z9connectToNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEi(&v3, 2, (int32_t)v2); // 0x401739
    int32_t v5 = v4; // 0x40173e
    _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev();
    _ZNSaIcED1Ev();
    if (v5 < 0) {
        // 0x401988
        return 0xffffffff;
    }
    int64_t v6 = function_401070(_Z14initTLSContextv(v2, 2, v2)); // 0x4017ab
    g3 = v6;
    if (v6 == 0) {
        int64_t v7 = _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(&g2, "Error creating SSL."); // 0x4017db
        _ZNSolsEPFRSoS_E((int64_t * (*)(int64_t *))v7);
        // 0x401988
        return 0xffffffff;
    }
    // 0x4017fe
    function_401050(v6, v5);
    int32_t v8 = function_401180(g3); // 0x40181e
    if (v8 >= 0 != v8 != 0) {
        int64_t v9 = function_4010c0(); // 0x40182b
        int64_t v10 = _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(&g2, "Error creating SSL connection. Error Code: "); // 0x401847
        int64_t v11 = function_401220((int32_t)v9, 0); // 0x401858
        _ZNSolsEPFRSoS_E((int64_t * (*)(int64_t *))_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc((int64_t *)v10, (char *)v11));
        // 0x401988
        return 0xffffffff;
    }
    // 0x4018e9
    if (function_4011e0(g3) == 0) {
        int64_t v12 = _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(&g1, "Call to SSL_get_verify_result is ok"); // 0x401914
        _ZNSolsEPFRSoS_E((int64_t * (*)(int64_t *))v12);
    }
    int64_t v13 = _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(&g1, "SSL communication established using "); // 0x40193f
    int64_t v14 = function_4010e0(function_4011c0(g3)); // 0x40195b
    _ZNSolsEPFRSoS_E((int64_t * (*)(int64_t *))_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc((int64_t *)v13, (char *)v14));
    // 0x401988
    return 0;
}

// --------------- Dynamically Linked Functions ---------------

// int64_t _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv(void);
// int64_t _ZNSaIcEC1Ev(void);
// int64_t _ZNSaIcED1Ev(void);
// int64_t _ZNSolsEPFRSoS_E(int64_t * (*a1)(int64_t *));
// int64_t _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_(char * a1, int64_t * a2);
// int64_t _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev(void);
// int64_t _ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE(int64_t * a1, int64_t * a2);
// int64_t _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(int64_t * a1, char * a2);
// int connect(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len);
// int64_t ERR_error_string(void);
// int64_t ERR_get_error(void);
// uint16_t htons(uint16_t hostshort);
// in_addr_t inet_addr(const char * cp);
// void * memset(void * s, int c, size_t n);
// int64_t OPENSSL_init_ssl(void);
// int printf(const char * restrict format, ...);
// int socket(int domain, int type, int protocol);
// int64_t SSL_CIPHER_get_name(void);
// int64_t SSL_connect(void);
// int64_t SSL_CTX_new(void);
// int64_t SSL_CTX_set_cipher_list(void);
// int64_t SSL_CTX_set_verify(void);
// int64_t SSL_get_current_cipher(void);
// int64_t SSL_get_verify_result(void);
// int64_t SSL_new(void);
// int64_t SSL_set_fd(void);
// int64_t TLSv1_2_client_method(void);

// --------------------- Meta-Information ---------------------

// Detected compiler/packer: gcc (9.3.0)
// Detected language: C++
// Detected functions: 21
