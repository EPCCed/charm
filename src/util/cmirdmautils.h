#ifndef _CKRDMAUTILS_H
#define _CKRDMAUTILS_H

#include "conv-header.h"

// Structure that can be used across layers
typedef struct ncpystruct{

  // Used in the MPI layer
#if CMK_CONVERSE_MPI
  char core[CmiMsgHeaderSizeBytes];
  int tag;
#endif

  const void *srcPtr;
  char *srcLayerInfo;
  char *srcAck;
  const void *srcRef;
  int srcPe;
  int srcSize;
  short int srcLayerSize;
  short int srcAckSize;
  unsigned char srcRegMode;
  unsigned char srcDeregMode;
  unsigned char isSrcRegistered;

  const void *destPtr;
  char *destLayerInfo;
  char *destAck;
  const void *destRef;
  int destPe;
  int destSize;
  short int destAckSize;
  short int destLayerSize;
  unsigned char destRegMode;
  unsigned char destDeregMode;
  unsigned char isDestRegistered;

  unsigned char opMode; // CMK_DIRECT_API for p2p direct api
                        // CMK_DIRECT_API_REVERSE for p2p direct api with inverse operation
                        // CMK_EM_API for p2p entry method api
                        // CMK_EM_API_REVERSE for p2p entry method api with inverse operation

  // Variables used for ack handling
  unsigned char ackMode; // CMK_SRC_DEST_ACK for call both src and dest acks
                         // CMK_SRC_ACK for call just src ack
                         // CMK_DEST_ACK for call just dest ack

  unsigned char freeMe;  // CMK_FREE_NCPYOPINFO in order to free NcpyOperationInfo
                         // CMK_DONT_FREE_NCPYOPINFO in order to not free NcpyOperationInfo

  short int ncpyOpInfoSize;

  int rootNode; // used only for Broadcast, -1 for p2p operations

  void *refPtr;

}NcpyOperationInfo;

#ifdef __cplusplus
extern "C" {
#endif

int getNcpyOpInfoTotalSize(
  int srcLayerSize,
  int srcAckSize,
  int destLayerSize,
  int destAckSize);

void setNcpyOpInfo(
  const void *srcPtr,
  char *srcLayerInfo,
  int srcLayerSize,
  char *srcAck,
  int srcAckSize,
  int srcSize,
  unsigned short int srcRegMode,
  unsigned short int srcDeregMode,
  unsigned short int isSrcRegistered,
  int srcPe,
  const void *srcRef,
  const void *destPtr,
  char *destLayerInfo,
  int destLayerSize,
  char *destAck,
  int destAckSize,
  int destSize,
  unsigned short int destRegMode,
  unsigned short int destDeregMode,
  unsigned short int isdestRegistered,
  int destPe,
  const void *destRef,
  int rootNode,
  NcpyOperationInfo *ncpyOpInfo);

void resetNcpyOpInfoPointers(NcpyOperationInfo *ncpyOpInfo);

void setReverseModeForNcpyOpInfo(NcpyOperationInfo *ncpyOpInfo);

#ifdef __cplusplus
}
#endif

#endif
