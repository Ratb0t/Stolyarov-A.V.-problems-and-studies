
set logging overwrite on
set pagination off
set confirm off
set listsize 1

set $program_started = 0

break main.c:14
break main.c:22

commands 2
set logging off
set var $program_started = 0
disable
end

run
    stepi
    stepi
    b *$rip
    commands 3 
        tb *(*((void**)($rsp)))
        continue
    end
    tb *(*((void**)($rsp)))
    continue
    stepi
    stepi
    stepi
    stepi
    b *$rip
    commands 5 
        tb *(*((void**)($rsp)))
        continue
    end

    set var $program_started = 1
    set logging file debug.log
    set logging on

    while $program_started
        if $_isvoid($_exitcode)  
            if !$_isvoid($rsp)
                list *(*(void**)($rsp))-5
            end
            continue
        else
            loop_break
        end
    end
quit