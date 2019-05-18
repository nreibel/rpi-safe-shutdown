#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/select.h>
#include<sys/time.h>

#define FALSE 0
#define TRUE !FALSE

#define HIGH '1'
#define LOW '0'

int read_int(char* from)
{
  char* endptr = NULL;
  int result = strtol(from, &endptr, 10);
  if (from == endptr)
  {
    return -1;
  }
  return result;
}

#define MAX_FILENAME_LENGTH 30

int main(int argc, char* argv[])
{
  char fname[MAX_FILENAME_LENGTH];
  char exit = FALSE;
  char buffer = HIGH;
  struct timeval timeout;
  int fd, pin, delay;
  fd_set fds;

  if (argc != 3)
  {
    fprintf(stderr, "Usage : gpio_wait <gpio> <delay>\n");
    return 1;
  }

  pin = read_int(argv[1]);
  delay = read_int(argv[2]);

  if (delay <= 0)
  {
    fprintf(stderr, "Invalid delay : must be a non-zero positive integer\n");
    return 1;
  }

  if (pin < 1 || pin > 26)
  {
    fprintf(stderr, "Invalid pin, must be an integer between 1 and 26\n");
    return 1;
  }

  sprintf(fname, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(fname, O_RDONLY);
  if (fd < 0)
  {
    fprintf(stderr, "Unable to read GPIO value\n");
    return 1;
  }

  while(!exit)
  {
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    timeout.tv_sec = delay;
    timeout.tv_usec = 0;

    if (0 == select(fd+1, NULL, NULL, &fds, &timeout))
    {
      // If the timeout triggers and the buffer was previously LOW,
      // it means the key hasn't been released
      if (buffer == LOW) exit = TRUE;
    }

    lseek(fd, 0, SEEK_SET);
    read(fd, &buffer, 1);
  }

  close(fd);

  return 0;
}
