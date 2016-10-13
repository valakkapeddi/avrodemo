#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avro.h>

void read_generic_avro(const char *avro_data_file) {
    avro_file_reader_t filereader;
    avro_value_t record;

    avro_file_reader(avro_data_file, &filereader);
    avro_schema_t schema = avro_file_reader_get_writer_schema(filereader);
    avro_value_iface_t* iface = avro_generic_class_from_schema(schema);
    
    avro_generic_value_new(iface, &record);

    while (avro_file_reader_read_value(filereader, &record) == 0) {
        avro_value_t field;
        const char *symbol;
        int  amount;
        double rate;
        long size;
        int side;
        
      
        avro_schema_t enum_schema = avro_schema_enum("Side");
        const char* blah = avro_schema_enum_get(schema, 1);
        
        
        
        avro_value_get_by_name(&record, "amount", &field, NULL);
        avro_value_get_int(&field, &amount);
        
        avro_value_get_by_name(&record, "rate", &field, NULL);
        avro_value_get_double(&field, &rate);
        
        avro_value_get_by_name(&record, "symbol", &field, NULL);
        avro_value_get_string(&field, &symbol, &size);
        
        avro_value_get_by_name(&record, "side", &field, NULL);
        avro_value_get_enum(&field, &side);
        

        printf("symbol: %s, amount: %d, rate: %f\n", symbol, amount, rate);
                
    }
    avro_file_reader_close(filereader);
    avro_value_decref(&record);
    avro_value_iface_decref(iface);
    avro_schema_decref(schema);
}


int main() {
    read_generic_avro("orders");

    return 0;
}
