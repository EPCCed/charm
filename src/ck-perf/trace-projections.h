/*****************************************************************************
 * $Source$
 * $Author$
 * $Date$
 * $Revision$
 *****************************************************************************/

/**
 * \addtogroup CkPerf
*/
/*@{*/

#ifndef _PROJECTIONS_H
#define _PROJECTIONS_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "trace.h"
#include "ckhashtable.h"

#if CMK_HAS_COUNTER_PAPI
#include <papi.h>
#endif

#if CMK_PROJECTIONS_USE_ZLIB
#include <zlib.h>
#endif

#include "pup.h"

#define PROJECTION_VERSION  "6.6"

// Macro to make projections check for errors before an fprintf succeeds.
#define CheckAndFPrintF(f,string,data) \
{ \
  int result = fprintf(f,string,data); \
  if (result == -1) { \
    CmiAbort("Projections I/O error!"); \
  } \
}

/// a log entry in trace projection
class LogEntry {
  public:
    double time;
    double cputime;
    double recvTime;
    int event;
    int pe;
    unsigned short mIdx;
    unsigned short eIdx;
    int msglen;
    CmiObjId   id;
    int numpes;
    int *pes;
    // this is taken out so as to provide a placeholder value for non-PAPI
    // versions (whose value is *always* zero).
    int numPapiEvents;
#if CMK_HAS_COUNTER_PAPI
    int *papiIDs;
    LONG_LONG_PAPI *papiValues;
#endif
    unsigned char type; 
    char *fName;
    int flen;
  public:
    LogEntry() {fName=NULL;flen=0;pes=NULL;numpes=0;}
    LogEntry(double tm, unsigned char t, unsigned short m=0, 
	     unsigned short e=0, int ev=0, int p=0, int ml=0, 
	     CmiObjId *d=NULL, double rt=0., double cputm=0., int numPe=0) {
      type = t; mIdx = m; eIdx = e; event = ev; pe = p; 
      time = tm; msglen = ml;
      if (d) id = *d; else {id.id[0]=id.id[1]=id.id[2]=0; };
      recvTime = rt; cputime = cputm;
      // initialize for papi as well as non papi versions.
      numPapiEvents = 0;
#if CMK_HAS_COUNTER_PAPI
      papiIDs = NULL;
      papiValues = NULL;
#endif
      fName = NULL; flen=0;
      pes=NULL;
      numpes=numPe;
    }
    LogEntry(double _time,unsigned char _type,unsigned short _funcID,
	     int _lineNum,char *_fileName){
      time = _time;
      type = _type;
      mIdx = _funcID;
      event = _lineNum;
      if(_fileName == NULL){
	fName = NULL;
	flen = 0;
      }else{
	fName = new char[strlen(_fileName)+2];
	fName[0] = ' ';
	memcpy(fName+1,_fileName,strlen(_fileName)+1);
	flen = strlen(fName)+1;
      }	
      pes=NULL;numpes=0;
    }
    // **CW** new constructor for multicast data
    LogEntry(double tm, unsigned short m, unsigned short e, int ev, int p,
	     int ml, CmiObjId *d, double rt, int numPe, int *pelist);
    // complementary function for adding papi data
    void addPapi( int numPapiEvts, int *papi_ids, LONG_LONG_PAPI *papiVals);
    void *operator new(size_t s) {void*ret=malloc(s);_MEMCHECK(ret);return ret;}
    void *operator new(size_t, void *ptr) { return ptr; }
    void operator delete(void *ptr) {free(ptr); }
#if defined(WIN32) || CMK_MULTIPLE_DELETE
    void operator delete(void *, void *) { }
#endif
    void pup(PUP::er &p);
    ~LogEntry(){
      if (fName) delete [] fName;
    }
};

class TraceProjections;

/// log pool in trace projection
class LogPool {
  friend class TraceProjections;
  friend class TraceProjectionsBOC;
  private:
    bool writeData;
    unsigned int poolSize;
    unsigned int numEntries;
    LogEntry *pool;
    FILE *fp;
    FILE *deltafp;
    FILE *stsfp;
    FILE *rcfp;
    char *fname;
    char *dfname;
    char *pgmname;
    int binary;
#if CMK_PROJECTIONS_USE_ZLIB
    gzFile deltazfp;
    gzFile zfp;
    int compressed;
#endif
    // **CW** prevTime stores the timestamp of the last event
    // written out to log. This allows the implementation of
    // simple delta encoding and should only be used when
    // writing out logs.
    double prevTime;
    double timeErr;
    double globalEndTime; // used at the end on Pe 0 only

    int headerWritten;
    bool fileCreated;
    void writeHeader();
  public:
    LogPool(char *pgm);
    ~LogPool();
    void setBinary(int b) { binary = b; }
#if CMK_PROJECTIONS_USE_ZLIB
    void setCompressed(int c) { compressed = c; }
#endif
    void createFile(char *fix="");
    void createSts(char *fix="");
    void createRC();
    void openLog(const char *mode);
    void closeLog(void);
    void writeLog(void);
    void write(int writedelta);
    void writeSts(void);
    void writeSts(TraceProjections *traceProj);
    void writeRC(void);

    void add(unsigned char type, unsigned short mIdx, unsigned short eIdx,
	     double time, int event, int pe, int ml=0, CmiObjId* id=0, 
	     double recvT=0., double cpuT=0.0, int numPe=0);
    // complementary function to set papi info to current log entry
    // must be called after an add()
    void addPapi(int numPap, int *pap_ids, LONG_LONG_PAPI *papVals) {
      pool[numEntries-1].addPapi(numPap, pap_ids, papVals);
    }
    void add(unsigned char type,double time,unsigned short funcID,int lineNum,char *fileName);
    void addCreationMulticast(unsigned short mIdx,unsigned short eIdx,double time,int event,int pe, int ml=0, CmiObjId* id=0, double recvT=0., int numPe=0, int *pelist=NULL);
    void flushLogBuffer();
    void postProcessLog();
};

/*
	class that represents a key in a CkHashtable with a string as a key
*/
class StrKey {
	char *str;
	int len;
	unsigned int key;
	public:
	StrKey(char *_str,int _len){
		str = _str;
		len = _len;
		key = 0;
		for(int i=0;i<len;i++){
			key += str[i];
		}
	}
	static CkHashCode staticHash(const void *k,size_t){
		return ((StrKey *)k)->key;
	}
	static int staticCompare(const void *a,const void *b,size_t){
		StrKey *p,*q;
		p = (StrKey *)a;
		q = (StrKey *)b;
		if(p->len != q->len){
			return 0;
		}
		for(int i=0;i<p->len;i++){
			if(p->str[i] != q->str[i]){
				return 0;
			}
		}
		return 1;
	}
	inline CkHashCode hash() const{
		return key;
	}
	inline int compare(const StrKey &t) const {
		if(len != t.len){
			return 0;
		}
		for(int i=0;i<len;i++){
			if(str[i] != t.str[i]){
				return 0;
			}	
		}
		return 1;
	}
	inline char *getStr(){
		return str;
	}
};

/// class for recording trace projections events 
/**
  TraceProjections will log Converse/Charm++ events and write into .log files;
  events descriptions will be written into .sts file.
*/
class TraceProjections : public Trace {
  friend class TraceProjectionsBOC;
 private:
    LogPool* _logPool;        /**<  logpool for all events */
    int curevent;
    int execEvent;
    int execEp;
    int execPe;
    int inEntry;
    int computationStarted;

    int funcCount;
    CkHashtableT<StrKey,int> funcHashtable;
    
    //as user now can specify the idx, it's possible that user may specify an existing idx
    //so that we need a data structure to track idx. --added by Chao Mei
    CkVec<int> idxVec;
    int idxRegistered(int idx);    
#if CMK_HAS_COUNTER_PAPI
    int papiEventSet;
    LONG_LONG_PAPI *papiValues;
#endif

  public:
    int converseExit; // used for exits that bypass CkExit.
    double endTime;

    TraceProjections(char **argv);
    void userEvent(int e);
    void userBracketEvent(int e, double bt, double et);
    void creation(envelope *e, int epIdx, int num=1);
    void creationMulticast(envelope *e, int epIdx, int num=1, int *pelist=NULL);
    void creationDone(int num=1);
    void beginExecute(envelope *e);
    void beginExecute(CmiObjId  *tid);
    void beginExecute(int event,int msgType,int ep,int srcPe,int ml,CmiObjId *idx=NULL);
    void endExecute(void);
    void messageRecv(char *env, int pe);
    void beginIdle(double curWallTime);
    void endIdle(double curWallTime);
    void beginPack(void);
    void endPack(void);
    void beginUnpack(void);
    void endUnpack(void);
    void enqueue(envelope *e);
    void dequeue(envelope *e);
    void beginComputation(void);
    void endComputation(void);

    int traceRegisterUserEvent(const char*, int);
    void traceClearEps();
    void traceWriteSts();
    void traceClose();
    void traceBegin();
    void traceEnd();
    void traceFlushLog() { _logPool->flushLogBuffer(); }

    //functions that perform function tracing
    CkHashtableIterator *getfuncIterator(){return funcHashtable.iterator();};
    int getFuncNumber(){return funcHashtable.numObjects();};
    void regFunc(const char *name, int &idx, int idxSpecifiedByUser=0);
    void beginFunc(char *name,char *file,int line);
    void beginFunc(int idx,char *file,int line);
    void endFunc(char *name);
    void endFunc(int num);

    /* This is for moving projections to being a charm++ module */
    void closeTrace(void);

    /* for overiding basic thread listener support in Trace class */
    virtual void traceAddThreadListeners(CthThread tid, envelope *e);
};

using namespace PUP;

class toProjectionsFile : public toTextFile {
 protected:
  virtual void bytes(void *p,int n,size_t itemSize,dataType t);
 public:
  //Begin writing to this file, which should be opened for ascii write.
  toProjectionsFile(FILE *f_) :toTextFile(f_) {}
};
class fromProjectionsFile : public fromTextFile {
 protected:
  virtual void bytes(void *p,int n,size_t itemSize,dataType t);
 public:
  //Begin writing to this file, which should be opened for ascii read.
  fromProjectionsFile(FILE *f_) :fromTextFile(f_) {}
};

#if CMK_PROJECTIONS_USE_ZLIB
class toProjectionsGZFile : public PUP::er {
  gzFile f;
 protected:
  virtual void bytes(void *p,int n,size_t itemSize,dataType t);
 public:
  //Begin writing to this gz file, which should be opened for gz write.
  toProjectionsGZFile(gzFile f_) :er(IS_PACKING), f(f_) {}
};
#endif


#endif

/*@}*/
