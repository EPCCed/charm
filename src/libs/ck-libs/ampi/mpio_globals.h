/* shared header between ROMIO and AMPI's implementation for global variables */

#ifndef MPIO_GLOBALS_H
#define MPIO_GLOBALS_H

struct ADIOI_Fl_node; /* typedef struct ADIOI_Fl_node ADIOI_Flatlist_node; */
struct ADIOI_Datarep; /* typedef struct ADIOI_Datarep ADIOI_Datarep; */
struct ADIOI_FileD; /* typedef struct ADIOI_FileD *ADIO_File; */
/* typedef MPI_Request ADIO_Request; */

struct ADIO_GlobalStruct {
    struct ADIOI_Fl_node * ADIOI_Flatlist;

    struct ADIOI_Datarep * ADIOI_Datarep_head;

    /* for f2c and c2f conversion */
    struct ADIOI_FileD ** ADIOI_Ftable;
    int ADIOI_Ftable_ptr;
    int ADIOI_Ftable_max;
    MPI_Request * ADIOI_Reqtable;
    int ADIOI_Reqtable_ptr;
    int ADIOI_Reqtable_max;
#ifndef HAVE_MPI_INFO
    MPI_Info *MPIR_Infotable;
    int MPIR_Infotable_ptr, MPIR_Infotable_max;
#endif
#if defined(ROMIO_XFS) || defined(ROMIO_LUSTRE) || 1
    int ADIOI_Direct_read, ADIOI_Direct_write;
#endif

    int ADIO_Init_keyval;

    MPI_Errhandler ADIOI_DFLT_ERR_HANDLER;

    MPI_Info ADIOI_syshints;

    MPI_Op ADIO_same_amode;

    int ADIOI_cb_config_list_keyval;

    char * yylval;
    char * token_ptr;
};

#ifdef __cplusplus
extern "C" {
#endif

void ADIO_Init_Globals(struct ADIO_GlobalStruct *);
struct ADIO_GlobalStruct * ADIO_Globals(void);
#define ADIO_Global(x) (ADIO_Globals()->x)

#ifdef __cplusplus
}
#endif

#endif
