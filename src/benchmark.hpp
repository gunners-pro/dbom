#pragma once
#include "database.hpp"
#include "utils/uuid.hpp"
#include <chrono>

inline void benchmark(Database& db, int count)
{
    db.use_collection("bench");

    using namespace std::chrono;

    // String JSON base
    std::string base_json = R"({"msg":"teste","i":0})";

    // Temporizadores separados
    auto total_start = high_resolution_clock::now();
    milliseconds total_parse{0}, total_insert{0}, total_save{0};

    for (int i = 0; i < count; ++i)
    {
        // Atualiza JSON simulando entrada dinâmica
        std::string json_input = R"({"msg":"teste","i":)" + std::to_string(i) + "}";

        // Parsing
        auto t1 = high_resolution_clock::now();
        // Document doc = Document::from_json(json_input);
        // doc.id = generate_uuidV4();
        auto t2 = high_resolution_clock::now();

        // Inserção em memória
        // db.current()->insert(doc);
        auto t3 = high_resolution_clock::now();

        total_parse += duration_cast<milliseconds>(t2 - t1);
        total_insert += duration_cast<milliseconds>(t3 - t2);
    }

    // Escrita no disco
    auto save_start = high_resolution_clock::now();
    db.current()->save();
    auto save_end = high_resolution_clock::now();
    total_save = duration_cast<milliseconds>(save_end - save_start);

    auto total_end = high_resolution_clock::now();
    auto total_ms = duration_cast<milliseconds>(total_end - total_start).count();

    std::cout << "Inseridos " << count << " documentos em " << total_ms << " ms\n";
    std::cout << (count * 1000.0 / total_ms) << " docs/seg\n\n";

    std::cout << "Tempo total de parse:  " << total_parse.count() << " ms\n";
    std::cout << "Tempo total de insert: " << total_insert.count() << " ms\n";
    std::cout << "Tempo total de save:   " << total_save.count() << " ms\n";
}
