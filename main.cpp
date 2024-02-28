#include <iostream>
#include "clickhouse/client.h"

void test1();

void test2();

int main() {
    std::cout << "Hello, World!" << std::endl;

    //test1();
    test2();

    return 0;
}

void test1(){
    using namespace clickhouse;

    /// Initialize client connection.
    Client client(ClientOptions().SetHost("192.168.216.86").SetPort(9001).SetUser("default").SetPassword("123456").SetCompressionMethod( CompressionMethod::LZ4));

    /// Create a table.
    client.Execute("CREATE TABLE IF NOT EXISTS default.numbers (id UInt64, name String) ENGINE = Memory");

    /// Insert some values.
    {
        Block block;

        auto id = std::make_shared<ColumnUInt64>();
        id->Append(1);
        id->Append(7);

        auto name = std::make_shared<ColumnString>();
        name->Append("one");
        name->Append("seven");

        block.AppendColumn("id"  , id);
        block.AppendColumn("name", name);

        client.Insert("default.numbers", block);
    }

    /// Select values inserted in the previous step.
    client.Select("SELECT id, name FROM default.numbers", [] (const Block& block)
                  {
                      for (size_t i = 0; i < block.GetRowCount(); ++i) {
                          std::cout << block[0]->As<ColumnUInt64>()->At(i) << " "
                                    << block[1]->As<ColumnString>()->At(i) << "\n";
                      }
                  }
    );

    /// Delete table.
    client.Execute("DROP TABLE default.numbers");
}

/**
CREATE TABLE IF NOT EXISTS test.test_ck_insert (
    id Int64,
    datetime DateTime,
    name String,
    cate LowCardinality(String),
    dims Array(Int32),
    count Int64
)
ENGINE=MergeTree
PARTITION BY toYYYYMMDD(datetime)
ORDER BY (datetime, id)
;

select * from test.test_ck_insert;
 */
void test2(){
    using namespace clickhouse;

    /// Initialize client connection.
    Client * client;
    try{
        client = new Client(ClientOptions().SetHost("192.168.216.86").SetPort(9001).SetUser("default").SetPassword("123456").SetCompressionMethod( CompressionMethod::LZ4));
        std::cout << "start:" << std::endl;
    }catch (const std::system_error& e){
        std::cout << "conn error:" << e.what() << std::endl;
        delete client;
        return;
    }


    /// Insert some values.
    {
        Block block;

        auto id = std::make_shared<ColumnInt64>();
        id->Append(1);
        id->Append(2);
        id->Append(3);

        auto datetime = std::make_shared<ColumnDateTime>();
        datetime->Append(1585670400);// new Date("2020-04-01 00:00:00").getTime() = 1585670400000
        datetime->Append(1585670401);
        datetime->Append(1585670402);

        auto name = std::make_shared<ColumnString>();
        name->Append("莫南");
        name->Append("燕青丝");
        name->Append("苏流沙");

        auto cate = std::make_shared<ColumnString>();
        cate->Append("a");
        cate->Append("b");
        cate->Append("a");

        auto dims = std::make_shared<ColumnArray>(std::make_shared<ColumnInt32>());
        auto dim = std::make_shared<ColumnInt32>();
        dim->Append(1);
        dim->Append(2);
        dims->AppendAsColumn(dim);

        dim->Clear();
        dim->Append(3);
        dim->Append(2);
        dims->AppendAsColumn(dim);

        dim->Clear();
        dim->Append(4);
        dims->AppendAsColumn(dim);

        auto count = std::make_shared<ColumnInt64>();
        count->Append(100);
        count->Append(200);
        count->Append(300);

        block.AppendColumn("id"  , id);
        block.AppendColumn("datetime", datetime);
        block.AppendColumn("name", name);
        block.AppendColumn("cate", cate);
        block.AppendColumn("dims", dims);
        block.AppendColumn("count", count);

        client->Insert("test.test_ck_insert", block);
    }

    /// Select values inserted in the previous step.
    client->Select("SELECT count(1) cnt FROM test.test_ck_insert", [] (const Block& block)
                  {
                      for (size_t i = 0; i < block.GetRowCount(); ++i) {
                          std::cout << "count:" << block[0]->As<ColumnUInt64>()->At(i) << "\n";
                      }
                  }
    );

    delete client;
}