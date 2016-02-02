/*
* msg.x: Remote message process protocol */
      program PROCESSORPROG {
          version PROCESSORVERS {
              int MSGPROCESSOR(string) = 1;
          } = 1;
} = 0x20006223;