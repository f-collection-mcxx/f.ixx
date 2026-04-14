import f;
import std;

int main() {
    // Test 1: first_t type alias
    {
        static_assert(std::same_as<f::first_t<int, double, float>, int>,
            "first_t should get the first type");
        std::println("✓ Test 1 passed: first_t gets first type");
    }

    // Test 2: last_t with multiple types
    {
        static_assert(std::same_as<f::last_t<int, double, float>, float>,
            "last_t should get the last type");
        std::println("✓ Test 2 passed: last_t gets last type");
    }

    // Test 3: last_t with single type
    {
        static_assert(std::same_as<f::last_t<int>, int>,
            "last_t with single type should return that type");
        std::println("✓ Test 3 passed: last_t with single type");
    }

    // Test 4: pack::pack_size
    {
        using p = f::pack<int, double, float, char>;
        if (p::pack_size == 4) {
            std::println("✓ Test 4 passed: pack_size is correct");
        } else {
            std::println("✗ Test 4 failed: pack_size should be 4, got {}", p::pack_size);
            return 1;
        }
    }

    // Test 5: pack::first
    {
        using p = f::pack<int, double, float>;
        static_assert(std::same_as<p::first, int>, "pack::first should be first type");
        std::println("✓ Test 5 passed: pack::first");
    }

    // Test 6: pack::last
    {
        using p = f::pack<int, double, float>;
        static_assert(std::same_as<p::last, float>, "pack::last should be last type");
        std::println("✓ Test 6 passed: pack::last");
    }

    // Test 7: pack::at<0>
    {
        using p = f::pack<int, double, float>;
        static_assert(std::same_as<p::at<0>, int>, "pack::at<0> should be first type");
        std::println("✓ Test 7 passed: pack::at<0>");
    }

    // Test 8: pack::at<1>
    {
        using p = f::pack<int, double, float>;
        static_assert(std::same_as<p::at<1>, double>, "pack::at<1> should be second type");
        std::println("✓ Test 8 passed: pack::at<1>");
    }

    // Test 9: pack::at<2>
    {
        using p = f::pack<int, double, float>;
        static_assert(std::same_as<p::at<2>, float>, "pack::at<2> should be third type");
        std::println("✓ Test 9 passed: pack::at<2>");
    }

    // Test 10: pack with single type
    {
        using p = f::pack<int>;
        if (p::pack_size == 1) {
            std::println("✓ Test 10 passed: single type pack_size");
        } else {
            std::println("✗ Test 10 failed");
            return 1;
        }
        static_assert(std::same_as<p::first, int>, "single type pack first");
        static_assert(std::same_as<p::last, int>, "single type pack last");
        static_assert(std::same_as<p::at<0>, int>, "single type pack at<0>");
        std::println("✓ Test 10 passed: single type pack");
    }

    std::println("\n✓ All pack tests passed!");
    return 0;
}
