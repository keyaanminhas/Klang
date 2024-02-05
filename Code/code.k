DECLARE Fact : INTEGER
DECLARE num : INTEGER
DECLARE name : STRING

OUTPUT "WHAT IS YOUR NAME"
INPUT name


OUTPUT "HELLO, ", name, "\n", "ENTER A NUMBER FOR FACTORIAL: "
INPUT num

Fact <-- 1


IF num < 0 THEN
    OUTPUT -1

ELSE IF num = 1 OR num = 0 THEN
    OUTPUT 1

ELSE
    FOR Count <-- 1 TO num
        Fact <-- Fact * Count
    NEXT
    OUTPUT Fact
ENDIF

DECLARE stop, start : INTEGER
stop <-- 5
start <-- 0
REPEAT
    OUTPUT start
    start <-- start + 1
UNTIL start == stop

DECLARE flag : BOOLEAN

flag <-- false

IF flag = false THEN
    OUTPUT "FLAG WAS FALSE"
ELSE
    OUTPUT "Flag was TRUE"
ENDIF