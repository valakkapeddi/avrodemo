#include "myschema.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"
#include "avro/ValidSchema.hh"
#include "avro/Compiler.hh"
#include "avro/DataFile.hh"
#include <iostream>
#include <fstream>

using namespace std;
using namespace avro;
using namespace akkapeddi;

int main() {
    
    ifstream schema_stream("myschema.avsc");
    ValidSchema my_schema;
    compileJsonSchema(schema_stream, my_schema);
    

    {
        DataFileWriter<LimitOrder> dfw("/var/tmp/order_from_cpp", my_schema);
        LimitOrder writeOrder;
        
        writeOrder.symbol="USDCAD";
        writeOrder.amount=1;
        writeOrder.rate=1.00;
        writeOrder.side = akkapeddi::SELL;
        dfw.write(writeOrder);
        
        dfw.close();
    }

    {
        DataFileReader<LimitOrder> dfr("/var/tmp/orders", my_schema);
        LimitOrder readOrder;
        while (dfr.read(readOrder)) {
            cout << '(' << readOrder.symbol << ", " << readOrder.rate << ", " << readOrder.side << ')' << endl;
        }
    }
    return 0;
}