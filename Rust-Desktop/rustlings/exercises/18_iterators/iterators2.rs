// In this exercise, you'll learn some of the unique advantages that iterators
// can offer.

// TODO: Complete the `capitalize_first` function.
// "hello" -> "Hello"
fn capitalize_first(input: &str) -> String {
    let mut chars = input.chars();
    match chars.next() {
        None => String::new(),
        Some(first) => first.to_uppercase().collect::<String>() + chars.as_str(),
    }
}

// TODO: Apply the `capitalize_first` function to a slice of string slices.
// Return a vector of strings.
// ["hello", "world"] -> ["Hello", "World"]
fn capitalize_words_vector(words: &[&str]) -> Vec<String> {
    // Use `iter` to create an iterator over the slice
    // - `iter` returns an iterator over references to the elements of the slice (it does not consume the slice)
    // Use `map` to apply the `capitalize_first` function to each element of the slice
    // Use `collect` to collect the results into a new vector

    words.iter().map(|word| capitalize_first(word)).collect()
}

// Alternative method to accomplish same result as `capitalize_words_vector`
fn capitalize_words_vector_loop_method(words: &[&str]) -> Vec<String> {
    let mut capitalized_words = Vec::new();
    // Iterate over the words and capitalize each one
    // The for...in loop is a shorthand for the `into_iterator` method
    // i.e. we loop over the iterator returned by `into_iterator`
    for word in words {
        capitalized_words.push(capitalize_first(word));
    }
    capitalized_words
}


// TODO: Apply the `capitalize_first` function again to a slice of string
// slices. Return a single string.
// ["hello", " ", "world"] -> "Hello World"
fn capitalize_words_string(words: &[&str]) -> String {
    capitalize_words_vector(words).join("")
}

fn main() {
    // You can optionally experiment here.
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_success() {
        assert_eq!(capitalize_first("hello"), "Hello");
    }

    #[test]
    fn test_empty() {
        assert_eq!(capitalize_first(""), "");
    }

    #[test]
    fn test_iterate_string_vec() {
        let words = vec!["hello", "world"];
        assert_eq!(capitalize_words_vector(&words), ["Hello", "World"]);
    }

    #[test]
    fn test_iterate_into_string() {
        let words = vec!["hello", " ", "world"];
        assert_eq!(capitalize_words_string(&words), "Hello World");
    }
}
