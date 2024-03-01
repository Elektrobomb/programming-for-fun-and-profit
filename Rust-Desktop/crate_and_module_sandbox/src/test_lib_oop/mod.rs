pub struct StringPrinter {
    string_to_print : String,
    should_print_with_newline : bool
}

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