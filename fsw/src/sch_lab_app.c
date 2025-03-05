/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as "core Flight System: Bootes"
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *  This file contains the source code for the SCH lab application
 */

/*
** Include Files
*/
#include <string.h>

#include "cfe.h"
#include "cfe_msgids.h"
#include "cfe_config.h"

#include "sch_lab_perfids.h"
#include "sch_lab_version.h"
#include "sch_lab_mission_cfg.h"
#include "sch_lab_tbl.h"

/*
** Global Structure
*/
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
    uint16                  MessageBuffer[SCH_LAB_MAX_ARGS_PER_ENTRY];
    uint16                  PayloadLength;
    uint32                  PacketRate;
    uint32                  Counter;
} SCH_LAB_StateEntry_t;

typedef struct
{
    SCH_LAB_StateEntry_t State[SCH_LAB_MAX_SCHEDULE_ENTRIES];
    osal_id_t            TimerId;
    osal_id_t            TimingSem;
    CFE_TBL_Handle_t     TblHandle;
    CFE_SB_PipeId_t      CmdPipe;
} SCH_LAB_GlobalData_t;

/*
** Global Variables
*/
SCH_LAB_GlobalData_t SCH_LAB_Global;

/* 
** Dummy Task Function Prototypes 
*/
void SCH_LAB_DummyTask1(void);
void SCH_LAB_DummyTask2(void);
void SCH_LAB_DummyTask3(void);
void SCH_LAB_DummyTask4(void);
void SCH_LAB_DummyTask5(void);

/*
** Local Function Prototypes
*/
CFE_Status_t SCH_LAB_AppInit(void);
void SCH_LAB_ProcessPacket(CFE_SB_Buffer_t *SBBufPtr);

/*
** AppMain
*/
void SCH_LAB_AppMain(void)
{
    uint32                RunStatus = CFE_ES_RunStatus_APP_RUN;
    CFE_Status_t          Status;
    uint32                TaskCounter = 0;
    CFE_SB_Buffer_t      *SBBufPtr;

    CFE_ES_PerfLogEntry(SCH_LAB_MAIN_TASK_PERF_ID);

    Status = SCH_LAB_AppInit();
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH_LAB: Error Initializing RC = 0x%08lX\n", (unsigned long)Status);
        RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /* Loop Forever */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(SCH_LAB_MAIN_TASK_PERF_ID);

        /* Wait for packet on command pipe */
        Status = CFE_SB_ReceiveBuffer(&SBBufPtr, SCH_LAB_Global.CmdPipe, CFE_SB_PEND_FOREVER);
        
        CFE_ES_PerfLogEntry(SCH_LAB_MAIN_TASK_PERF_ID);

        if (Status == CFE_SUCCESS)
        {
            /* Process the received packet */
            SCH_LAB_ProcessPacket(SBBufPtr);
            
            /* Increment task counter */
            TaskCounter++;
            CFE_ES_WriteToSysLog("SCH_LAB: Completed cycle %u\n", (unsigned int)TaskCounter);
        }
        else
        {
            CFE_ES_WriteToSysLog("SCH_LAB: Error receiving packet: 0x%08lX\n", (unsigned long)Status);
        }
    }

    CFE_ES_ExitApp(RunStatus);
}

/*
** Process received packet and execute the appropriate task
*/
void SCH_LAB_ProcessPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId;
    uint8_t TaskId = 0;
    
    /* Get message ID and function code */
    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);
    
    CFE_ES_WriteToSysLog("SCH_LAB: Received MsgId: 0x%04X\n", 
                         (unsigned int)CFE_SB_MsgIdToValue(MsgId));
    
        /* Get the task ID from the first byte of the payload */
    uint8_t *payload = (uint8_t *)SBBufPtr + 8; /* Skip the 8-byte header */
    TaskId = payload[0];
        
        /* Use the task ID from the payload */
    switch (TaskId)
    {
        case 1:
            SCH_LAB_DummyTask1();
            break;
             
        case 2:
            SCH_LAB_DummyTask2();
            break;
              
        case 3:
            SCH_LAB_DummyTask3();
            break;
               
        case 4:
            SCH_LAB_DummyTask4();
            break;
                
        case 5:
            SCH_LAB_DummyTask5();
            break;
                
        default:
            CFE_ES_WriteToSysLog("SCH_LAB: Unknown task ID from payload: %d\n", TaskId);
        break;
    }
}

/* 
** Dummy Task Implementations 
*/
void SCH_LAB_DummyTask1(void)
{
    OS_time_t start_time;
    OS_time_t current_time;
    OS_time_t elapsed_time;
    OS_time_t wait_time = OS_TimeFromTotalMilliseconds(50); /* 50ms */
    
    OS_GetLocalTime(&start_time);
    
    /* Busy wait loop */
    do
    {
        OS_GetLocalTime(&current_time);
        elapsed_time = OS_TimeSubtract(current_time, start_time);
    } while (OS_TimeCompare(elapsed_time, wait_time) < 0);
}

void SCH_LAB_DummyTask2(void)
{
    OS_time_t start_time;
    OS_time_t current_time;
    OS_time_t elapsed_time;
    OS_time_t wait_time = OS_TimeFromTotalMilliseconds(75); /* 75ms */
    
    OS_GetLocalTime(&start_time);
    
    /* Busy wait loop */
    do
    {
        OS_GetLocalTime(&current_time);
        elapsed_time = OS_TimeSubtract(current_time, start_time);
    } while (OS_TimeCompare(elapsed_time, wait_time) < 0);
}

void SCH_LAB_DummyTask3(void)
{
    OS_time_t start_time;
    OS_time_t current_time;
    OS_time_t elapsed_time;
    OS_time_t wait_time = OS_TimeFromTotalMilliseconds(125); /* 125ms */
    
    OS_GetLocalTime(&start_time);
    
    /* Busy wait loop */
    do
    {
        OS_GetLocalTime(&current_time);
        elapsed_time = OS_TimeSubtract(current_time, start_time);
    } while (OS_TimeCompare(elapsed_time, wait_time) < 0);
}

void SCH_LAB_DummyTask4(void)
{
    OS_time_t start_time;
    OS_time_t current_time;
    OS_time_t elapsed_time;
    OS_time_t wait_time = OS_TimeFromTotalMilliseconds(100); /* 100ms */
    
    OS_GetLocalTime(&start_time);
    
    /* Busy wait loop */
    do
    {
        OS_GetLocalTime(&current_time);
        elapsed_time = OS_TimeSubtract(current_time, start_time);
    } while (OS_TimeCompare(elapsed_time, wait_time) < 0);
}

void SCH_LAB_DummyTask5(void)
{
    OS_time_t start_time;
    OS_time_t current_time;
    OS_time_t elapsed_time;
    OS_time_t wait_time = OS_TimeFromTotalMilliseconds(150); /* 150ms */
    
    OS_GetLocalTime(&start_time);
    
    /* Busy wait loop */
    do
    {
        OS_GetLocalTime(&current_time);
        elapsed_time = OS_TimeSubtract(current_time, start_time);
    } while (OS_TimeCompare(elapsed_time, wait_time) < 0);
}

void SCH_LAB_LocalTimerCallback(osal_id_t object_id, void *arg)
{
    OS_CountSemGive(SCH_LAB_Global.TimingSem);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialization                                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t SCH_LAB_AppInit(void)
{
    CFE_Status_t                  Status;
    char                          VersionString[SCH_LAB_CFG_MAX_VERSION_STR_LEN];

    memset(&SCH_LAB_Global, 0, sizeof(SCH_LAB_Global));

    /* Create command pipe */
    Status = CFE_SB_CreatePipe(&SCH_LAB_Global.CmdPipe, 16, "SCH_LAB_CMD_PIPE");
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH_LAB: Error creating command pipe, RC = 0x%08lX\n", (unsigned long)Status);
        return Status;
    }

    /* Subscribe to command packets */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0x1801), SCH_LAB_Global.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH_LAB: Error subscribing to command 0x1801, RC = 0x%08lX\n", (unsigned long)Status);
        return Status;
    }
    
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0x1802), SCH_LAB_Global.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH_LAB: Error subscribing to command 0x1802, RC = 0x%08lX\n", (unsigned long)Status);
        return Status;
    }
    
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0x1803), SCH_LAB_Global.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH_LAB: Error subscribing to command 0x1803, RC = 0x%08lX\n", (unsigned long)Status);
        return Status;
    }
    
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0x1804), SCH_LAB_Global.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH_LAB: Error subscribing to command 0x1804, RC = 0x%08lX\n", (unsigned long)Status);
        return Status;
    }
    
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(0x1805), SCH_LAB_Global.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH_LAB: Error subscribing to command 0x1805, RC = 0x%08lX\n", (unsigned long)Status);
        return Status;
    }

    CFE_Config_GetVersionString(VersionString, SCH_LAB_CFG_MAX_VERSION_STR_LEN, "SCH Lab",
                          SCH_LAB_VERSION, SCH_LAB_BUILD_CODENAME, SCH_LAB_LAST_OFFICIAL);

    OS_printf("SCH Lab Initialized to receive commands for 5 dummy tasks.%s\n", VersionString);

    return CFE_SUCCESS;
}
