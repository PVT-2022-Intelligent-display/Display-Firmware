/*
 * extProcComm.h
 *
 *	Functions for reading incoming data from external processor (= data coming over uart2)
 *
 *  Created on: May 18, 2022
 *      Author: Nouth
 */

#ifndef INC_EXTPROCCOMM_H_
#define INC_EXTPROCCOMM_H_

/*
 * Checks whether there is a message from external processor (on uart2) waiting to be handled, if so, handles it.
 *
 * Currently supports only 1 kind of message - displaying text on an interactive label object.
 *
 */
int handleIncomingProcessorMsg(int objectCount, struct object *objectArray, uint8_t **pointerArray);

#endif /* INC_EXTPROCCOMM_H_ */
