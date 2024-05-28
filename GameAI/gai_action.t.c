#include <gai_action.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
////

static int SolderActions_IdleCleanup(gai_action_t * action, void* userdata)
{
    printf("SolderActions_IdleCleanup\n");
    return 0;
}

static int SolderActions_IdleInitialize(gai_action_t * action, void* userdata)
{
    printf("SolderActions_IdleInitialize\n");
    return 0;
}

static int SolderActions_IdleUpdate(gai_action_t * action, void* userdata)
{
    printf("SolderActions_IdleUpdate\n");
    return GAI_ACTION_STATUS_TERMINATED;
}

////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv)
{
    gai_action_t SoldierActions_IdleAction;
    
    gai_action_init(&SoldierActions_IdleAction, "Soldier_IdleAct"
                    , SolderActions_IdleInitialize, SolderActions_IdleUpdate, SolderActions_IdleCleanup, 0);
    
    gai_action_initialize(&SoldierActions_IdleAction, 0);
    gai_action_update(&SoldierActions_IdleAction, 0);
    gai_action_cleanup(&SoldierActions_IdleAction, 0);
    
    
    return 0;
}