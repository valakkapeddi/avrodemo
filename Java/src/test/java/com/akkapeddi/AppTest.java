package com.akkapeddi;


import com.akkapeddi.sample.LimitOrder;
import com.akkapeddi.sample.Side;
import org.apache.avro.Schema;
import org.apache.avro.file.DataFileReader;
import org.apache.avro.file.DataFileWriter;
import org.apache.avro.generic.GenericDatumReader;
import org.apache.avro.io.*;
import org.apache.avro.reflect.ReflectData;
import org.apache.avro.reflect.ReflectDatumWriter;
import org.apache.avro.specific.SpecificDatumReader;
import org.apache.avro.specific.SpecificDatumWriter;
import org.junit.Test;

import java.io.*;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.StreamSupport;
import static org.junit.Assert.assertEquals;

public class AppTest
{
    LimitOrder order1 = new LimitOrder("EURUSD", 500000, 1.11d, Side.SELL);

    @Test
    public void serializeMessages() throws IOException {
        Path path = Paths.get("/var/tmp/orders");

        /* Note that toString on any Avro-generated class will return a valid JSON String.
        / However, due to encoding differences, this may not produce JSON that is deserializable
        / For more complex Avro schemas, such as those using unions.
        / For those cases, it is advised that you use the JSON Encoder if your schemas use unions.*/
        String order1AsJson = order1.toString();
        System.out.println(order1AsJson);

        DatumWriter<LimitOrder> orderWriter = new SpecificDatumWriter<>(LimitOrder.class);
        DataFileWriter<LimitOrder> fileWriter = new DataFileWriter<>(orderWriter);

        final File file = path.toFile();
        fileWriter.create(order1.getSchema(), file);
        fileWriter.append(order1);

        System.out.println(file.getAbsolutePath());

        fileWriter.close();
    }

    @Test
    public void readMessages() throws IOException {
        DatumReader<LimitOrder> dataReader = new SpecificDatumReader<>(LimitOrder.class);
        final File file = Paths.get("/var/tmp/order").toFile();

        DataFileReader<LimitOrder> reader = new DataFileReader<>(file, dataReader);

        final List<LimitOrder> collect = StreamSupport.stream(reader.spliterator(), false).collect(Collectors.toList());

        assertEquals(order1, collect.get(0));
    }

    @Test
    public void readMessagesFromCpp() throws IOException {
        DatumReader<LimitOrder> dataReader = new SpecificDatumReader<>(LimitOrder.class);
        final File file = Paths.get("/var/tmp/order_from_cpp").toFile();

        DataFileReader<LimitOrder> reader = new DataFileReader<>(file, dataReader);

        final LimitOrder collect = StreamSupport.stream(reader.spliterator(), false).collect(Collectors.toList()).get(0);

        assertEquals(collect.getSymbol().toString(), "USDCAD");
        assertEquals(collect.getSide(), Side.SELL);


    }

    @Test
    public void serializeMessagesToJson() throws IOException {
        LimitOrder order1 = new LimitOrder("EURUSD", 500000, 1.11d, Side.BUY);
        order1.getSchema();

        OutputStream outstream = new ByteArrayOutputStream();
        final JsonEncoder jsonEncoder = new EncoderFactory().jsonEncoder(order1.getSchema(), outstream);

        DatumWriter<LimitOrder> orderWriter = new ReflectDatumWriter<>(order1.getSchema());

        orderWriter.write(order1, jsonEncoder);
        jsonEncoder.flush();

        String jsonOrder = outstream.toString();
        System.out.println(jsonOrder);
    }

    @Test
    public void deserializeJsonData() throws IOException {
        final String jsonString = order1.toString();
        InputStream inputStream = new ByteArrayInputStream(jsonString.getBytes());
        Schema schema = ReflectData.get().getSchema(LimitOrder.class);

        JsonDecoder decoder = new DecoderFactory().jsonDecoder(schema, inputStream);
        DatumReader<LimitOrder> reader = new SpecificDatumReader<>(schema);

        final LimitOrder limitOrder1 = reader.read(null, decoder);

    }


}

