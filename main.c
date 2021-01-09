#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#include <linux/gpio.h>
//#include <gpiod.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// SPI0 is being used
static const char *device = "/dev/spidev0.0";

// SPI Mode set to 0 (WHY??)
static uint8_t mode = 0;
static uint8_t bitsPerWord = 8;
static uint32_t clockSpeed = 50000;

uint32_t loop = 0;

// Helper function to print and abort
static void printAbort(const char *msg)
{
    perror(msg);
    abort();
}

int main(int argc, char **argv)
{
    int status = 0;

    // Opens the spidev linux file as read and write. Returns the file descriptor. See: https://man7.org/linux/man-pages/man2/open.2.html
    int fileDescriptor = open(device, O_RDWR);
    if (fileDescriptor < 0)
        printAbort("Device cannot be accessed.");

    // Setting the options for the Linux SPI File Descriptor

    // Modifies the file descriptor. See: https://www.man7.org/linux/man-pages/man2/ioctl.2.html
    status = ioctl(fileDescriptor, SPI_IOC_WR_MODE, &mode);
    if (status == -1)
        printAbort("Can't set SPI mode.");

    status = ioctl(fileDescriptor, SPI_IOC_RD_MODE, &mode);
    if (status == -1)
        printAbort("Can't get SPI mode.");

    status = ioctl(fileDescriptor, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord);
    if (status == -1)
    {
        printf("Bits = %d\n", bitsPerWord);
        printAbort("Can't set SPI bits per word");
    }

    status = ioctl(fileDescriptor, SPI_IOC_RD_BITS_PER_WORD, &bitsPerWord);
    if (status == -1)
        printAbort("Can't get SPI bits per word");

    status = ioctl(fileDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &clockSpeed);
    if (status == -1)
        printAbort("Can't set SPI max speed hz");

    status = ioctl(fileDescriptor, SPI_IOC_RD_MAX_SPEED_HZ, &clockSpeed);
    if (status == -1)
        printAbort("Can't get SPI max speed hz");

    uint8_t tx[] = {
        0x09};
    uint8_t rx[2] = {
        0,
    };

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = ARRAY_SIZE(tx) + ARRAY_SIZE(rx),
        .delay_usecs = 0,
        .speed_hz = clockSpeed,
        .bits_per_word = bitsPerWord,
    };

    while (loop < 50000)
    {
        loop++;

        // Write the transfer struct into the file descriptor
        status = ioctl(fileDescriptor, SPI_IOC_MESSAGE(1), &tr);
        if (status < 1)
            printAbort("can't send spi message");

        printf("%d%d", rx[0], rx[1]);
        printf("\n");
    }

    close(fileDescriptor);

    printf("Operation finished \n");
    return 0;
}