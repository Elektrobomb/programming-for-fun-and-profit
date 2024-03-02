pub struct StringPrinter {
    string_to_print : String,
    should_print_with_newline : bool
}
// Allows you to make a new StringPrinter with default values, set a string, print the string or get the string
impl StringPrinter {
    pub fn new() -> Self {
        Self {
            string_to_print: String::new(),
            should_print_with_newline: true,
        }
    }
    pub fn print_string(&self) -> () {
        print!("{}",self.string_to_print);

        if self.should_print_with_newline {
            print!("\n");
        }
    }

    pub fn set_string(&mut self, new_string : String) -> () {
        self.string_to_print = new_string.clone();
    }

    pub fn get_string(&self) -> &String {
        return &self.string_to_print
    }

    pub fn set_should_print_with_newline(&mut self, should_print_with_newline : bool) -> () {
        self.should_print_with_newline = should_print_with_newline;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_new_string_printer() {
        let printer = StringPrinter::new();
        assert_eq!(printer.get_string(), "");
        assert_eq!(printer.should_print_with_newline, true);
    }

    #[test]
    fn test_print_string_with_newline() {
        let mut printer = StringPrinter::new();
        printer.set_string("Hello, world!".to_string());
        printer.print_string();
        // Expected output: "Hello, world!\n"
    }

    #[test]
    fn test_print_string_without_newline() {
        let mut printer = StringPrinter::new();
        printer.set_string("Hello, world!".to_string());
        printer.set_should_print_with_newline(false);
        printer.print_string();
        // Expected output: "Hello, world!"
    }

    #[test]
    fn test_set_string() {
        let mut printer = StringPrinter::new();
        printer.set_string("Hello, world!".to_string());
        assert_eq!(printer.get_string(), "Hello, world!");
    }

    #[test]
    fn test_set_should_print_with_newline() {
        let mut printer = StringPrinter::new();
        printer.set_should_print_with_newline(false);
        assert_eq!(printer.should_print_with_newline, false);
    }
}
