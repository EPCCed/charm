/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/* 
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "adio.h"
#include "adio_extern.h"

/* AMPI: Global variable initialization moved to ampi.C */

static void my_consensus(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    int i, *in, *inout;
    in = (int*)invec;
    inout = (int*)inoutvec;

    for (i=0; i< *len; i++) {
        if (in[i] != inout[i])
	    inout[i] = ADIO_AMODE_NOMATCH;
    }
    return;
}

void ADIO_Init(int *argc, char ***argv, int *error_code)
{
#if defined(ROMIO_XFS) || defined(ROMIO_LUSTRE)
    char *c;
#endif

    ADIOI_UNREFERENCED_ARG(argc);
    ADIOI_UNREFERENCED_ARG(argv);

/* initialize the linked list containing flattened datatypes */
    ADIO_Global(ADIOI_Flatlist) = (ADIOI_Flatlist_node *) ADIOI_Malloc(sizeof(ADIOI_Flatlist_node));
    ADIO_Global(ADIOI_Flatlist)->type = MPI_DATATYPE_NULL;
    ADIO_Global(ADIOI_Flatlist)->next = NULL;
    ADIO_Global(ADIOI_Flatlist)->blocklens = NULL;
    ADIO_Global(ADIOI_Flatlist)->indices = NULL;

#if defined(ROMIO_XFS) || defined(ROMIO_LUSTRE)
    c = getenv("MPIO_DIRECT_READ");
    if (c && (!strcmp(c, "true") || !strcmp(c, "TRUE"))) 
	ADIOI_Direct_read = 1;
    else ADIOI_Direct_read = 0;
    c = getenv("MPIO_DIRECT_WRITE");
    if (c && (!strcmp(c, "true") || !strcmp(c, "TRUE"))) 
	ADIOI_Direct_write = 1;
    else ADIOI_Direct_write = 0;
#endif


#ifdef ADIOI_MPE_LOGGING
    {
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_open_a, &ADIOI_MPE_open_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_read_a, &ADIOI_MPE_read_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_write_a, &ADIOI_MPE_write_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_lseek_a, &ADIOI_MPE_lseek_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_close_a, &ADIOI_MPE_close_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_writelock_a,
                                    &ADIOI_MPE_writelock_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_readlock_a,
                                    &ADIOI_MPE_readlock_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_unlock_a, &ADIOI_MPE_unlock_b );
        MPE_Log_get_state_eventIDs( &ADIOI_MPE_postwrite_a,
                                    &ADIOI_MPE_postwrite_b );
	MPE_Log_get_state_eventIDs( &ADIOI_MPE_openinternal_a, 
			&ADIOI_MPE_openinternal_b);
	MPE_Log_get_state_eventIDs( &ADIOI_MPE_stat_a, &ADIOI_MPE_stat_b);
	MPE_Log_get_state_eventIDs( &ADIOI_MPE_iread_a, &ADIOI_MPE_iread_b);
	MPE_Log_get_state_eventIDs( &ADIOI_MPE_iwrite_a, &ADIOI_MPE_iwrite_b);

        int  comm_world_rank;
        MPI_Comm_rank( MPI_COMM_WORLD, &comm_world_rank );

        if ( comm_world_rank == 0 ) {
            MPE_Describe_state( ADIOI_MPE_open_a, ADIOI_MPE_open_b,
                                "open", "orange" );
            MPE_Describe_state( ADIOI_MPE_read_a, ADIOI_MPE_read_b,
                                "read", "green" );
            MPE_Describe_state( ADIOI_MPE_write_a, ADIOI_MPE_write_b,
                                "write", "blue" );
            MPE_Describe_state( ADIOI_MPE_lseek_a, ADIOI_MPE_lseek_b,
                                "lseek", "red" );
            MPE_Describe_state( ADIOI_MPE_close_a, ADIOI_MPE_close_b,
                                "close", "grey" );
            MPE_Describe_state( ADIOI_MPE_writelock_a, ADIOI_MPE_writelock_b,
                                "writelock", "plum" );
            MPE_Describe_state( ADIOI_MPE_readlock_a, ADIOI_MPE_readlock_b,
                                "readlock", "magenta" );
            MPE_Describe_state( ADIOI_MPE_unlock_a, ADIOI_MPE_unlock_b,
                                "unlock", "purple" );
            MPE_Describe_state( ADIOI_MPE_postwrite_a, ADIOI_MPE_postwrite_b,
                                "postwrite", "ivory" );
	    MPE_Describe_state( ADIOI_MPE_openinternal_a, ADIOI_MPE_openinternal_b, "open system", "blue");
	    MPE_Describe_state( ADIOI_MPE_stat_a, ADIOI_MPE_stat_b, "stat", "purple");
	    MPE_Describe_state( ADIOI_MPE_iread_a, ADIOI_MPE_iread_b, "iread", "purple");
	    MPE_Describe_state( ADIOI_MPE_iwrite_a, ADIOI_MPE_iwrite_b, "iwrite", "purple");
        }
    }
#endif

    *error_code = MPI_SUCCESS;
    MPI_Op_create(my_consensus, 1, &ADIO_Global(ADIO_same_amode));
}
