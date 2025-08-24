#include "__string_tester.h"

/**
 *
 *     FUNCTIONS TO BE TESTED
 *  => [V] String_ins_cstr(PString self, CStr str, size_t beg)
 *  => [V] String_ins_str(PString self, Str str, size_t beg, size_t Nbytes)
 *  => [V] String_ins_Str(PString self, RFString str, size_t beg)
 *  => [V] String_ins_Str_range(PString self, RFString str, size_t beg, size_t NBytes, size_t off)
 *
**/

#define TEST_FN(fn, s, ...)\
        do{\
            fn(&(s), __VA_ARGS__);\
            if(__impl_equstr((s).ptr, __exp))\
                 fn_status[idx] = FNS_FUNCTION_SUCCED;\
            else fn_status[idx] = FNS_FUNCTION_FAILED;\
            __impl_tester_log(#fn, idx++, (s).ptr, __exp);\
        }while(0)

bool is_atty =      false;
const int n_total = 4;
int fn_status[4];
int tester_header_length = 60;

int main(void){
    __impl_memset(fn_status, FNS_FUNCTION_FAILED, sizeof(fn_status));
    size_t idx = 0;

    String s1 = String_new_from_cstr("HelWorld");
    String s2 = String_new_from_cstr("HelWorld");
    String s3 = String_new_from_cstr("HelWorld");
    String s4 = String_new_from_cstr("HelWorld");
    CStr lo =   "lo ";
    Str lo_sv = "Hello World";
    String lo_Str = String_new_from_cstr("lo ");
    String Str = String_new_from_cstr("Hello World");

    const char* __exp = NULL;
    START_TEST( );
    
        __exp = "Hello World";
        TEST_FN(String_ins_cstr, s1, lo, 3);
        
        __exp = "Hello World";
        TEST_FN(String_ins_sv, s2, &lo_sv[3], 3, 3);

        __exp = "Hello World";
        TEST_FN(String_ins_Str, s3, &lo_Str, 3);
        
        __exp = "Hello World";
        TEST_FN(String_ins_Str_range, s4, &Str, 3, 3, 3);

    END_TEST( );

    return 0;
}
