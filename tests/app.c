// #include <string.h>
// #define STRNCAT strncat
#include "__string_tester.h"

/**
 * 
 *     FUNCTIONS TO BE TESTED
**  => [V] String_app_cstr(PString self, CStr cstr)
**  => [V] String_app_sv(PString self, Str str, size_t Nbytes)
**  => [V] String_app_sv_range(PString self, Str _str, size_t offset, size_t Nbytes)
**  => [V] String_app_Str(PString self, RFString str)
**  => [V] String_app_nStr(PString self, RFString str, size_t Nbytes)
**  => [V] String_app_Str_range(PString self, RFString str, size_t offset, size_t Nbytes)
 *  => [X] String_push_char(PString self, char ch)
 *  => [X] String_app_word(PString self, Str word, size_t Nbytes)
**/
#define __FOUND(_str) fprintf(stderr, "   %s -> %s\n", MK_BHMAGENTA_TXT("Found"), (_str)->ptr);        

#define __APP_TESTER(_func, _res, _str, ...)\
        do{\
            CStr res = _res;\
            (void) _func ((_str), __VA_ARGS__ );\
            if(String_cmp_cstr((_str), _res) == STRING_EQUAL){\
                FUNC_SUCCEED(#_func)\
            }else{\
                FUNC_FAILED(#_func)\
                __FOUND(_str)\
            }\
        }while(0)

#define TEST_FN(fn, s, ...)\
        do{\
            fn(&(s), __VA_ARGS__);\
            if(STREQU((s).ptr, __exp))\
                 fn_status[idx] = FNS_FUNCTION_SUCCED;\
            else fn_status[idx] = FNS_FUNCTION_FAILED;\
            __impl_tester_log(#fn, idx++, (s).ptr, __exp);\
        }while(0)

bool is_atty =              false;
const int n_total =         8;
int fn_status[8];
int tester_header_length =  60;

size_t idx = 0;

int main(void){
    MEMSET(fn_status, 1, sizeof(fn_status));

    String s1 = String_new( );
    String s2 = String_new_from_cstr("Hello World");
    String s3 = String_new_from_cstr("Hello");
    String s4 = String_new_from_cstr("Hello");
    
    const char* __exp = NULL;
    Str sv_range = "Help me World";
    START_TEST( );
        __exp = "Hello from Appenders";
        TEST_FN(String_app_cstr, s1, __exp);
        
        __exp = "Hello WorldHello";
        TEST_FN(String_app_sv, s2, "Hello from Appenders", 5);
        
        __exp   = "HelloHello from Appenders";
        TEST_FN(String_app_Str, s3, &s1); /* s3 = "Hello", s1 = "Hello World" */

        __exp = "Hello World";
        TEST_FN(String_app_sv_range, s4, sv_range, 6, 7); /* s4 = "Hello", sv_range = " World" */

        String_shrink_to(&s1, 6);
        __exp = "Hello Hello";
        TEST_FN(String_app_nStr, s1, &s2, 5);
        
        String_shrink_to(&s1, 6);
        __exp = "Hello World";
        TEST_FN(String_app_Str_range, s1, &s2, 5, 6);
        
        String_shrink_to(&s1, 5);
        __exp = "Hello Please";
        sv_range = "Please help me!";
        TEST_FN(String_app_word, s1, sv_range, 6);
        
        __exp = "Hello Please!";
        TEST_FN(String_push_char, s1, '!');
        
    END_TEST( );

    return 0;
}