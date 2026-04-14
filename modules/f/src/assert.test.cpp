import f;
import std;

int main() {
    // Test 1: assert with true condition should not throw
    try {
        f::assert(true);
        std::println("✓ Test 1 passed: assert(true) does not throw");
    } catch (...) {
        std::println("✗ Test 1 failed: assert(true) should not throw");
        return 1;
    }

    // Test 2: assert with false condition should throw exception
    try {
        f::assert(false);
        std::println("✗ Test 2 failed: assert(false) should throw");
        return 1;
    } catch (const std::runtime_error& e) {
        std::println("✓ Test 2 passed: assert(false) throws exception: {}", e.what());
    }

    // Test 3: assert with expression that evaluates to true
    try {
        int x = 5;
        f::assert(x == 5);
        std::println("✓ Test 3 passed: assert with true expression");
    } catch (...) {
        std::println("✗ Test 3 failed");
        return 1;
    }

    // Test 4: assert with expression that evaluates to false
    try {
        int x = 5;
        f::assert(x == 10);
        std::println("✗ Test 4 failed: assert with false expression should throw");
        return 1;
    } catch (const std::runtime_error& e) {
        std::println("✓ Test 4 passed: assert with false expression throws: {}", e.what());
    }

    std::println("\n✓ All assert tests passed!");
    return 0;
}
