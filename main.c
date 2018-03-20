#include "Windows.h"
#include "inttypes.h"

// This is a CryptoNite parameter which determines the size of the memory
// buffer to allocate.
uint32_t MEMORY = (1 << 21);

// This is determines whether AES HW is used (if it is present), use parameter "disable_AES_HW" to set this to 1
int g_disable_AES_HW = 0; // By default, use AES HW if it is present

int main(int argc, char* argv[])
{
	char hash_in[128], hash_out[128];
	
	int variant = 0; // Monero CryptoNite variant
	
	int prehashed = 0; // Data given the the CryptoNite algo is not pre-hasehd

	int N = 20; // Number of runs to perform - will collect stats on this

	__int64 sum = 0, sq_sum = 0; // used to collect stats 

	int print_after_hash_memory = 0, print_before_hash_memory = 0;

	memset(hash_out, 0, sizeof(hash_out)); // Initialize the memory to hash

	printf("AES H/W support is present:\t%s\n", check_aes_hw() ? "TRUE" : "FALSE");

	if (argc > 1) // check for the disable_AES_HW parameter on the command line
	{
		for (int iarg = 1; iarg < argc; iarg++)
		{
			if (strcmp(argv[iarg], "disable_AES_HW") == 0)
			{
				g_disable_AES_HW = 1;
				printf("AES_HW support disabled\n");
			}
		}
	}

	printf("N hashes:\t%ld\n", N);

	// Print the starting data - 128 bytes of zeros
	if (print_before_hash_memory)
	{
		for (int i = 0; i < 128; i++)
		{
			if (i % 16 == 0)
				printf("\n[%02X] ", i);

			printf("%02X ", (uint8_t)hash_out[i]);
		}
	}

	printf("\n");


	for (int memory_bits = 24; memory_bits > 10; memory_bits--)
	{
		MEMORY = (1 << memory_bits); // Set the CryptoNite memory for the current run, must start high and move down or we get an exception since memory is only allocated once

		sum = 0;
		sq_sum = 0;

		for (int i = 0; i < N; i++)
		{
			// The last hash becomes data for the next hash
			memmove(hash_in, hash_out, sizeof(hash_in));

			// Time collection values
			SYSTEMTIME st0, st1; // Windows specific
			FILETIME ft0, ft1;   // Windows specific
			int64_t t0, t1;

			int elapsed_ms = 0;

			// Get the start time for the hash run - Windows specific code here
			GetSystemTime(&st0);

			// CryptoNite hashing algorithm - taken from Monero codebase on 3/19/2018
			cn_slow_hash(hash_in, sizeof(hash_in), hash_out, variant, prehashed);

			// Get the end time for the hash run - Windows specific
			GetSystemTime(&st1);

			// Convert times to int64 in 100 ns increments - Windows specific
			SystemTimeToFileTime(&st0, &ft0);
			SystemTimeToFileTime(&st1, &ft1);

			memmove(&t0, &ft0, sizeof(t0));
			memmove(&t1, &ft1, sizeof(t1));

			// Convert delta time from 100 ns increments to ms increments
			elapsed_ms = (int)(t1 - t0);
			elapsed_ms /= 10000;

			// Collect data for stats
			sum += elapsed_ms;
			sq_sum += elapsed_ms * elapsed_ms;
		}

		// Print the final hash after N runs through the algo
		if (print_after_hash_memory)
		{
			for (int i = 0; i < 128; i++)
			{
				if (i % 16 == 0)
					printf("\n[%02X] ", i);

				printf("%02X ", (uint8_t)hash_out[i]);
			}
		}

		// Print out the stats
		{
			double avg = (double)sum / (double)N;
			double stdev = sqrt(((double)sq_sum - (double)sum*(double)sum / (double)N) / (double)(N - 1));

			if (memory_bits ==24) // print the header before the first data line
				printf("Memory(KB)\tAVG ms/hash\tSTD\n");

			printf("%ld\t%3.1lf\t%4.2lf\n", MEMORY / 1024, avg, stdev);
		}
	}

	printf("\n");


	return 0;
}






