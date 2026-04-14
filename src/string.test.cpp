import f;
import std;

int main() {
    // Test 1: format_with_resource with custom memory resource
    {
        std::pmr::monotonic_buffer_resource buffer;
        auto result = f::format("{} + {} = {}", &buffer, 1, 2, 3);
        if (result == "1 + 2 = 3") {
            std::println("✓ Test 1 passed: format");
        } else {
            std::println("✗ Test 1 failed: expected '1 + 2 = 3', got '{}'", result);
            return 1;
        }
    }

    // Test 2: format without memory resource (uses default)
    {
        auto result = f::format("Hello, {}!", "World");
        if (result == "Hello, World!") {
            std::println("✓ Test 2 passed: format without resource");
        } else {
            std::println("✗ Test 2 failed: expected 'Hello, World!', got '{}'", result);
            return 1;
        }
    }

    // Test 3: format with memory resource as first argument
    {
        std::pmr::monotonic_buffer_resource buffer;
        auto result = f::format("{} {}", &buffer, "test", "format");
        if (result == "test format") {
            std::println("✓ Test 3 passed: format with resource as first arg");
        } else {
            std::println("✗ Test 3 failed: expected 'test format', got '{}'", result);
            return 1;
        }
    }

    // Test 4: cvt string to wstring (view version)
    {
        std::wstring buf;
        f::cvt("hello", buf);
        if (buf == L"hello") {
            std::println("✓ Test 4 passed: cvt string_view to wstring");
        } else {
            std::println("✗ Test 4 failed");
            return 1;
        }
    }

    // Test 5: cvt string to wstring (string version)
    {
        std::pmr::string input = "world";
        auto result = f::cvt(input);
        if (result == L"world") {
            std::println("✓ Test 5 passed: cvt string to wstring");
        } else {
            std::println("✗ Test 5 failed");
            return 1;
        }
    }

    // Test 6: cvt wstring to string (view version)
    {
        std::string buf;
        f::cvt(L"hello", buf);
        if (buf == "hello") {
            std::println("✓ Test 6 passed: cvt wstring_view to string");
        } else {
            std::println("✗ Test 6 failed");
            return 1;
        }
    }

    // Test 7: cvt wstring to string (wstring version)
    {
        std::pmr::wstring input = L"world";
        auto result = f::cvt(input);
        if (result == "world") {
            std::println("✓ Test 7 passed: cvt wstring to string");
        } else {
            std::println("✗ Test 7 failed");
            return 1;
        }
    }

    // Test 8: format with different types
    {
        auto result = f::format("int: {}, double: {:.2f}", 42, 3.14159);
        if (result.find("42") != std::string::npos && result.find("3.14") != std::string::npos) {
            std::println("✓ Test 8 passed: format with different types");
        } else {
            std::println("✗ Test 8 failed: got '{}'", result);
            return 1;
        }
    }

    std::println("\n✓ All string tests passed!");
    return 0;
}
