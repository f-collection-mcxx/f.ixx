import f;
import std;

int main() {
    // Test 1: Hash string with string_hasher
    {
        f::string_hasher hasher;
        std::string s1 = "hello";
        std::string s2 = "hello";
        std::string s3 = "world";

        size_t h1 = hasher(s1);
        size_t h2 = hasher(s2);
        size_t h3 = hasher(s3);

        if (h1 == h2) {
            std::println("✓ Test 1 passed: same strings produce same hash");
        } else {
            std::println("✗ Test 1 failed: same strings should have same hash");
            return 1;
        }

        if (h1 != h3) {
            std::println("✓ Test 2 passed: different strings likely produce different hash");
        } else {
            std::println("⚠ Test 2 warning: different strings have same hash (collision)");
        }
    }

    // Test 2: Hash with string_view
    {
        f::string_hasher hasher;
        std::string_view sv1 = "test";
        std::string_view sv2 = "test";

        size_t h1 = hasher(sv1);
        size_t h2 = hasher(sv2);

        if (h1 == h2) {
            std::println("✓ Test 3 passed: string_view hashing works");
        } else {
            std::println("✗ Test 3 failed: string_view hash mismatch");
            return 1;
        }
    }

    // Test 3: Hash with C-string
    {
        f::string_hasher hasher;
        const char* cstr = "c-string";

        size_t h = hasher(cstr);
        size_t h_expected = hasher(std::string_view{"c-string"});

        if (h == h_expected) {
            std::println("✓ Test 4 passed: C-string hashing works");
        } else {
            std::println("✗ Test 4 failed: C-string hash mismatch");
            return 1;
        }
    }

    // Test 4: Use in unordered_map with transparent hasher
    {
        std::unordered_map<std::string, int, f::string_hasher, std::equal_to<>> map;
        map["key1"] = 100;
        map["key2"] = 200;

        std::string_view lookup = "key1";
        if (map.contains(lookup) && map.find(lookup)->second == 100) {
            std::println("✓ Test 5 passed: unordered_map with string_hasher works");
        } else {
            std::println("✗ Test 5 failed: unordered_map lookup failed");
            return 1;
        }
    }

    // Test 5: is_transparent property
    {
        static_assert(std::is_same_v<typename f::string_hasher::is_transparent, void>,
            "string_hasher should have is_transparent");
        std::println("✓ Test 6 passed: string_hasher has is_transparent");
    }

    std::println("\n✓ All hash tests passed!");
    return 0;
}
