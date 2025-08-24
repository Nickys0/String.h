
#include "__string_tester.h"

/**
 *   FUNCTIONS TO BE TESTED => Comparator
 *     => [V]String_equ_cstr(RFString self, CStr str1);
 *     => [V]String_equ_sv(RFString self, Str str1, size_t s1Len);  // comparing self and str1 for Nbytes
 *     => [V]String_equ_Str(RFString self, RFString str1);
 *     => [V]String_equ_oStr(RFString self, RFString str1, size_t offset); // Compares the two string but str1 starts from offset
 *     => [V]String_bequ_oStr(RFString self, RFString str1, size_t offset, size_t Nbytes); 
 *     => [V]String_bequ_Str(RFString self, RFString str1, size_t Nbytes); // Compares NBytes of the two strings
 *     => [V]String_bequ_cstr //alias
 *     => [V]String_bequ_sv //alias
 *     => [X]String_iequ_cstr(RFString self, CStr str1);
 *     => [X]String_iequ_sv(RFString self, Str str1, size_t Nbytes);
 *     => [X]String_iequ_Str(RFString self, RFString str1);
 *     => [X]String_iequ_oStr(RFString self, RFString str1, size_t offset);
 *     => [X]String_ibequ_oStr(RFString self, RFString str1, size_t offset, size_t Nbytes)
 *     => [X]String_ibequ_Str(RFString self, RFString str1, size_t _Nbytes)
 *     => [X]String_ibequ_cstr //alias
 *     => [X]String_ibequ_str //alias
**/

#define TEST_FN(fn, s, ...)\
        do{\
            if(fn (&(s), __VA_ARGS__) == true)\
                fn_status[idx] = FNS_FUNCTION_SUCCED;\
            else\
                fn_status[idx] = FNS_FUNCTION_FAILED;\
        __impl_tester_log(#fn, idx++, "FALSE", "TRUE");\
        }while(0)

/* we don't need to the the aliases */
#define FUNC_COUNT (16 - 4)
bool is_atty = false;
const int n_total = FUNC_COUNT;
int fn_status[FUNC_COUNT];
int tester_header_length = 60;

int main(void){
    MEMSET(fn_status, FNS_FUNCTION_FAILED, sizeof(fn_status));
    String s1 = String_new_from_cstr("Hello World");
    String s2 = String_dupl(&s1);
    String s3 = String_new_from_cstr("World");
    String s4 = String_new_from_cstr("hello world");
    String s5 = String_new_from_cstr("world");
    String s6 = String_new_from_cstr("HelloWorld");
    String s7 = String_new_from_cstr("World");
    size_t idx = 0;
    
    START_TEST( );
        /* case sensitive */
        TEST_FN(String_equ_cstr, s1, "Hello World");
        TEST_FN(String_equ_sv, s1, "HelloWorld", 5);
        TEST_FN(String_equ_Str, s1, &s2);
        TEST_FN(String_equ_oStr, s1, &s3, 6); /* "Hello |World" == "World" */
        
        /* case insensitive */
        TEST_FN(String_iequ_cstr, s1, "hello world");
        TEST_FN(String_iequ_sv, s1, "helloworld", 5);
        TEST_FN(String_iequ_Str, s1, &s4);
        TEST_FN(String_iequ_oStr, s1, &s5, 6);

        /* case sensitive comparison of N bytes */
        // TEST_FN(String_bequ_cstr, s1, "HelloWorld", 5); /* we don't need testing this for the moment */
        // TEST_FN(String_bequ_sv, s1, "HelloWorld", 5); /* we don't need testing this for the moment */
        TEST_FN(String_bequ_Str, s1, &s6, 5);
        TEST_FN(String_bequ_oStr, s7, &s1, 6, 5);
        
        /* case insensitive comparison of N bytes */
        // TEST_FN(String_ibequ_cstr, s1, "HelloWorld", 5); /* we don't need testing this for the moment */
        // TEST_FN(String_ibequ_sv, s1, "HelloWorld", 5); /* we don't need testing this for the moment */
        TEST_FN(String_ibequ_Str, s5, &s3, 5);
        TEST_FN(String_ibequ_oStr, s5, &s1, 6, 5);
        



    END_TEST( );

    return 0;
}

