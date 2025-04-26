define tar
    target extended-remote localhost:3000
end

define rtos list_tasks
    set $pxList = pxReadyTasksLists
    set $priority = 0
    while $priority < configMAX_PRIORITIES
        set $pxTCBList = $pxList[$priority].xListEnd.pxNext
        while $pxTCBList != &$pxList[$priority].xListEnd
            set $pxTCB = (TCB_t *)((char *)$pxTCBList - offsetof(TCB_t, xStateListItem))
            printf "Task: %s | Priority: %d | State: %d | Stack: 0x%08x | SP: 0x%08x\n", \
                $pxTCB->pcTaskName, \
                $pxTCB->uxPriority, \
                $pxTCB->eCurrentState, \
                $pxTCB->pxStack, \
                $pxTCB->pxTopOfStack
            set $pxTCBList = $pxTCBList->pxNext
        end
        set $priority = $priority + 1
    end
end

define rtos stack_usage
    set $tcb = pxCurrentTCB
    set $sp = $tcb->pxTopOfStack
    set $stack_start = $tcb->pxStack
    printf "Task: %s\n", $tcb->pcTaskName
    printf "Stack used: %d bytes\n", (char*)$sp - (char*)$stack_start
end

define rtos current_task
    set $tcb = pxCurrentTCB
    printf "Current Task:\n"
    printf "Name: %s | Priority: %d | Stack: 0x%08x | SP: 0x%08x\n", \
        $tcb->pcTaskName, $tcb->uxPriority, $tcb->pxStack, $tcb->pxTopOfStack
end

define stm reset_cause
    set $csr = *((unsigned int *)0x40023874)
    printf "RCC->CSR = 0x%08x\n", $csr
    if ($csr & (1 << 26))
        printf "Reset cause: IWDG (Independent Watchdog)\n"
    end
    if ($csr & (1 << 27))
        printf "Reset cause: WWDG (Window Watchdog)\n"
    end
    if ($csr & (1 << 28))
        printf "Reset cause: Low-Power Reset\n"
    end
    if ($csr & (1 << 29))
        printf "Reset cause: Software Reset\n"
    end
    if ($csr & (1 << 30))
        printf "Reset cause: POR/PDR Reset\n"
    end
    if ($csr & (1 << 31))
        printf "Reset cause: Pin Reset (NRST)\n"
    end
end