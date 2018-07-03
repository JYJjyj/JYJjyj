#include <stdio.h>
#include "parse.h"
#include "extern.h"
 int cmd_read()
 {
     return 0;
 }
int cmd_parse()
{
    return 0;
}
int cmd_execute()
{
    return 0;
}
void shell_loop()
{
    while(1)
    {
        cmd_read();
        cmd_parse();
        cmd_execute();
    }
}
