import f;
import std;

int main() {
    // Test 1: Create exception with simple message
    {
        auto ex = f::exception("simple error");
        std::string msg = ex.what();
        if (msg == "simple error") {
            std::println("✓ Test 1 passed: simple exception message");
        } else {
            std::println("✗ Test 1 failed: expected 'simple error', got '{}'", msg);
            return 1;
        }
    }

    // Test 2: Create exception with format and one argument
    {
        int code = 404;
        auto ex = f::exception("error code: {}", code);
        std::string msg = ex.what();
        if (msg == "error code: 404") {
            std::println("✓ Test 2 passed: exception with one argument");
        } else {
            std::println("✗ Test 2 failed: expected 'error code: 404', got '{}'", msg);
            return 1;
        }
    }

    // Test 3: Create exception with format and multiple arguments
    {
        std::string file = "test.txt";
        int line = 42;
        auto ex = f::exception("error in {} at line {}", file, line);
        std::string msg = ex.what();
        if (msg == "error in test.txt at line 42") {
            std::println("✓ Test 3 passed: exception with multiple arguments");
        } else {
            std::println("✗ Test 3 failed: expected 'error in test.txt at line 42', got '{}'", msg);
            return 1;
        }
    }

    // Test 4: Throw and catch exception
    {
        try {
            throw f::exception("throw test");
        } catch (const std::runtime_error& e) {
            if (std::string{e.what()} == "throw test") {
                std::println("✓ Test 4 passed: throw and catch exception");
            } else {
                std::println("✗ Test 4 failed: wrong message");
                return 1;
            }
        }
    }

    // Test 5: Exception with different types
    {
        double value = 3.14;
        auto ex = f::exception("pi value: {:.2f}", value);
        std::string msg = ex.what();
        if (msg.find("3.14") != std::string::npos) {
            std::println("✓ Test 5 passed: exception with double value");
        } else {
            std::println("✗ Test 5 failed: expected '3.14' in '{}'", msg);
            return 1;
        }
    }

    std::println("\n✓ All exception tests passed!");
    return 0;
}
