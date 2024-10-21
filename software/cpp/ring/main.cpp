// #include <iostream>
// #include <stdio.h>
// #include "ring_buffer.h"
// using namespace std;


// int main()
// {
//   RingBuffer bfr = RingBuffer();
//   printf("Initial buffer:\n");
//   bfr.print_buffer();
//   printf("Filling up past capacity without reading\n");
//   for(uint8_t i = 0; i < BFR_SIZE+1; i++)
//   {
//     bfr.write(i+1);
//     bfr.print_buffer();
//     printf("Full ? %d, empty ? %d\n", bfr.full(), bfr.empty());
//     if(bfr.n != bfr.available())
//     {
//       printf("ERROR in available bytes\n");
//       exit(1);
//     }
//   }
//   printf("Reading a single element: %02x\n", bfr.read());
//   printf("Writing 0x42 ? %d\n", bfr.write(0x42));
//   printf("Reading up past capacity without writing\n");
//   for(uint8_t i = 0; i < BFR_SIZE+1; i++)
//   {
//     printf("Got: %02x\n",bfr.read());
//     bfr.print_buffer();
//     printf("Full ? %d, empty ? %d\n", bfr.full(), bfr.empty());
//     if(bfr.n != bfr.available())
//     {
//       printf("ERROR in available bytes\n");
//       exit(1);
//     }
//   }
//   return 0;
// }
#include <iostream>
#include "ring_buffer.h"
#include <cassert>
void test_empty_buffer() {
    RingBuffer rb;
    assert(rb.empty() == true && "Buffer should be empty after initialization");
    assert(rb.available() == 0 && "Available bytes should be 0 in an empty buffer");
    std::cout << "test_empty_buffer passed.\n";
}

void test_write_to_buffer() {
    RingBuffer rb;
    assert(rb.write(0x01) == true && "Writing to the buffer should succeed");
    assert(rb.empty() == false && "Buffer should not be empty after writing");
    assert(rb.available() == 1 && "Available bytes should be 1 after writing 1 byte");
    std::cout << "test_write_to_buffer passed.\n";
}

void test_read_from_buffer() {
    RingBuffer rb;
    rb.write(0x01);
    uint8_t data = rb.read();
    assert(data == 0x01 && "Read data should match what was written");
    assert(rb.empty() == true && "Buffer should be empty after reading the only byte");
    assert(rb.available() == 0 && "Available bytes should be 0 after reading");
    std::cout << "test_read_from_buffer passed.\n";
}

void test_full_buffer() {
    RingBuffer rb;
    for (int i = 0; i < BFR_SIZE - 1; i++) {
        assert(rb.write(i) == true && "Writing to the buffer should succeed while it is not full");
    }
    assert(rb.full() == true && "Buffer should be full after writing BFR_SIZE - 1 elements");
    assert(rb.write(0xFF) == false && "Writing to a full buffer should fail");
    std::cout << "test_full_buffer passed.\n";
}

void test_buffer_wrap_around() {
    RingBuffer rb;
    for (int i = 0; i < BFR_SIZE - 1; i++) {
        rb.write(i);
    }
    for (int i = 0; i < BFR_SIZE / 2; i++) {
        rb.read();
    }
    for (int i = 0; i < BFR_SIZE / 2; i++) {
        assert(rb.write(i + 100) == true && "Buffer should allow wrap-around writing");
    }
    std::cout << "test_buffer_wrap_around passed.\n";
}

void test_available_bytes() {
    RingBuffer rb;
    rb.write(0x01);
    rb.write(0x02);
    assert(rb.available() == 2 && "Available bytes should be 2 after writing 2 elements");
    rb.read();
    assert(rb.available() == 1 && "Available bytes should be 1 after reading 1 element");
    std::cout << "test_available_bytes passed.\n";
}

void run_tests() {
    test_empty_buffer();
    test_write_to_buffer();
    test_read_from_buffer();
    test_full_buffer();
    test_buffer_wrap_around();
    test_available_bytes();
    std::cout << "All tests passed.\n";
}

int main() {
    run_tests();
    return 0;
}

