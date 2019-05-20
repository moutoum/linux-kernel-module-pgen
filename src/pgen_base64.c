#include <linux/types.h>

static const char alpha_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				    "abcdefghijklmnopqrstuvwxyz" \
				    "0123456789+/";

// pgen_base64_len returns the size of the base64 encoded payload depending
// on the data_size.
size_t pgen_base64_len(size_t data_size)
{
	return (((data_size / 3) + !!(data_size % 3)) * 4);
}

// pgen_base64_encode transforms the binary data into base64 encoding.
// The buffer should have enought memory space.
void pgen_base64_encode(const unsigned char *cdata,
			size_t data_size,
			char *buffer)
{
	size_t 		len;
	unsigned char 	from[3];
	unsigned char	to[4];
	size_t		j;

	j = 0;
	for (len = 0; len / 3 < data_size / 3; len += 3, j += 4) {

		from[0] = cdata[len    ];
		from[1] = cdata[len + 1];
		from[2] = cdata[len + 2];

		to[0] = from[0] >> 2;
		to[1] = ((from[0] & 0x3) << 4) | (from[1] >> 4);
		to[2] = ((from[1] & 0xF) << 2) | (from[2] >> 6);
		to[3] = from[2] & 0x3F;

		buffer[j    ] = alpha_table[to[0]];
		buffer[j + 1] = alpha_table[to[1]];
		buffer[j + 2] = alpha_table[to[2]];
		buffer[j + 3] = alpha_table[to[3]];
	}

	switch (data_size % 3) {
	case 1:
		from[0] = cdata[len];

		to[0] = from[0] >> 2;
		to[1] = (from[0] & 0x3) << 4;
		
		buffer[j    ] = alpha_table[to[0]];
		buffer[j + 1] = alpha_table[to[1]];
		buffer[j + 2] = '=';
		buffer[j + 3] = '=';
		j += 4;
		break;
	
	case 2:
		from[0] = cdata[len    ];
		from[1] = cdata[len + 1];

		to[0] = from[0] >> 2;
		to[1] = ((from[0] & 0x3) << 4) | (from[1] >> 4);
		to[2] = (from[1] & 0xF) << 2;

		buffer[j    ] = alpha_table[to[0]];
		buffer[j + 1] = alpha_table[to[1]];
		buffer[j + 2] = alpha_table[to[2]];
		buffer[j + 3] = '=';
		j += 4;
		break;

	default:
		break;
	}

	buffer[j] = '\0';
}
