#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_IMPLEMENTATION
#define NOB_TEMP_CAPACITY (32*1024) /* 32Kb should be enough */
#include "nob.h"

void how_to_use(void){
	fprintf(stdout, "./bil <command>\n");
	fprintf(stdout, "Example:\n");
	fprintf(stdout, "   ./bil test app\n");
}

bool comp_run_test(const char* basename){
    
    Nob_String_View fsv = nob_sv_from_cstr(basename);
    fsv = nob_sv_chop_by_delim(&fsv, '.');

    const char* file =  nob_temp_sprintf("tests/%.*s.c", SV_Arg(fsv));
    char* out =         nob_temp_sprintf("tests/result/%.*s",  SV_Arg(fsv));

    Nob_Builder __b = {
        .t_file = file,
        .f_out = out,
        .args = &(Nob_Builder_Args){
            .Warnings = NOB_ARG_LIST("-Werror=incompatible-pointer-types"),
            .others = NOB_ARG_LIST("-ggdb", "-DSTRING")
        }
    };

    if(!nob_comp_c_file_to_exe_and_run(__b)) return false;
    return true;
}

bool handle_args(int argc, char** argv){
    NOB_ASSERT(argv != NULL);

    if(strcmp(argv[1], "run") == 0){
        if(argc > 2) return comp_run_test(argv[2]);
        else {
            nob_elog("Expected a value but found NULL: ./bil run <test_file>\n");
            return 1;
        }

	}else if(strcmp(argv[1], "clean") == 0){
        Nob_File_Paths paths = {0};

        if(!nob_read_entire_dir("tests/result", &paths, .add_parent_path = true)) return false;

        for (size_t i = 0; i < paths.count; i++){
            if(!nob_delete_file(paths.items[i])) return false;   
        }

    }else if(strcmp(argv[1], "test") == 0){
        Nob_File_Paths paths = {0};

        if(!nob_get_dir_files_by_ext("tests", ".c", &paths)) return false;

        for(size_t i = 0; i < paths.count; i++){
            // nob_ilog("SRCFILE: %s", paths.items[i]);
            if(!comp_run_test(nob_path_name(paths.items[i]))) return false;
        }
    }else{
		nob_elog("Unkown command at argv[1]: %s", argv[1]);
		how_to_use();
	}
    
    return true;
}

int main(int argc, char** argv){
    NOB_GO_REBUILD_URSELF(argc, argv);
    nob_minimal_log_level = NOB_WARNING;

    if(!nob_mkdir_if_not_exists_rec("tests/result")) return 1;
    
    if(argc > 1){
        handle_args(argc, argv);
    }else	how_to_use();
    
    return 0;
}
