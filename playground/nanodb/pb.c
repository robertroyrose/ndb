bool checkreturn pb_encode_varint(pb_ostream_t *stream, uint64_t value)
{
    pb_byte_t buffer[10];
    int i;

    do {
	    if (value > 127) {
		    buffer[i++] = (value & 0x7f) | 0x80;
	    } else {
		    buffer[i++] = (value & 0x7f);
	    }
	    value >>= 7;
    } while (value);

    return pb_write(stream, buffer, i);
}


bool checkreturn pb_encode_svarint(pb_ostream_t *stream, pb_int64_t value)
{       
    pb_uint64_t zigzagged;
    if (value < 0)
        zigzagged = ~((pb_uint64_t)value << 1);
    else
        zigzagged = (pb_uint64_t)value << 1;
            
    return pb_encode_varint(stream, zigzagged);
}           
