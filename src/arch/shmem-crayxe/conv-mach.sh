#CMK_DEFS="$CMK_DEFS -I/opt/xt-mpt/1.5.47/mpich2-64/T/include"
#CMK_LD_DEFS="-lrca "

PGCC=`CC -V 2>/dev/null | grep pgCC`

CMK_INC="-I/opt/cray/mpt/5.2.0/xt/gemini/sma/include/"
CMK_LD_PATH="-L/opt/cray/mpt/5.2.0/xt/gemini/sma/lib64/"

CMK_CPP_CHARM="cpp -P"
CMK_CPP_C="cc"
CMK_CC="cc"
CMK_CXX="CC"
CMK_LD="$CMK_CC"
CMK_LDXX="$CMK_CXX"

CMK_CPP_C_FLAGS="-E $CMK_INC"
CMK_CC_FLAGS="$CMK_INC"
CMK_CXX_FLAGS="$CMK_INC"
CMK_LD_FLAGS="$CMK_CC_FLAGS $CMK_LD_DEFS $CMK_LD_PATH"
CMK_LDXX_FLAGS="$CMK_CXX_FLAGS $CMK_LD_DEFS $CMK_LD_PATH"

CMK_LIBS="-lckqt -lsma -lrca"
CMK_QT='generic64-light'

CMK_RANLIB="ranlib"
CMK_LD_LIBRARY_PATH="-Wl,-rpath,$CHARMLIBSO/"

# compiler for compiling sequential programs
if test -n "$PGCC"
then
CMK_CC_FLAGS="$CMK_CC_FLAGS -DCMK_FIND_FIRST_OF_PREDICATE=1 "
CMK_CXX_FLAGS="$CMK_CXX_FLAGS -DCMK_FIND_FIRST_OF_PREDICATE=1 --no_using_std "
# gcc is needed for building QT
CMK_SEQ_CC="gcc -fPIC"
CMK_SEQ_CXX="pgCC -fPIC --no_using_std "
CMK_COMPILER='pgcc'
else
CMK_SEQ_CC="gcc -fPIC"
CMK_SEQ_CXX="g++ -fPIC "
CMK_COMPILER='gcc'
fi
CMK_SEQ_LD="$CMK_SEQ_CC "
CMK_SEQ_LDXX="$CMK_SEQ_CXX "
CMK_SEQ_LIBS=""

CMK_SEQ_CC_FLAGS=' '
CMK_SEQ_CXX_FLAGS=' '
CMK_SEQ_LD_FLAGS=' '
CMK_SEQ_LDXX_FLAGS=' '

# compiler for native programs
CMK_NATIVE_CC="gcc "
CMK_NATIVE_LD="gcc "
CMK_NATIVE_CXX="g++ "
CMK_NATIVE_LDXX="g++ "
CMK_NATIVE_LIBS=""

CMK_NATIVE_CC_FLAGS=' '
CMK_NATIVE_CXX_FLAGS=' '
CMK_NATIVE_LD_FLAGS=' '
CMK_NATIVE_LDXX_FLAGS=' '


# for F90 compiler
CMK_CF77="ftn "
CMK_CF90="ftn "
CMK_F90LIBS=""
CMK_F90_USE_MODDIR=1
CMK_F90_MODINC="-I"
CMK_MOD_EXT="mod"

CMK_NO_BUILD_SHARED=true
CMK_NO_PARTITIONS="1"
