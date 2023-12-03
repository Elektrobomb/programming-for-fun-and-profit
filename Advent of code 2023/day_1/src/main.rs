use std::{
    fs::File,
    io::{prelude::*, BufReader},
    path::Path,
};
use regex::Regex;

fn main() {
    let data = lines_from_file("./data/input_matt.txt");

    let mut sum_calibration_vals : i32 = 0;
    for s in data{
        let first_last_digits = get_first_and_last_digits_in_str(&s);
        let digit_concat : i32 = format!("{}{}", first_last_digits.0, first_last_digits.1).parse().unwrap();
        println!("input: {}, first_num = {}, last_num = {}, sum = {}", s, first_last_digits.0, first_last_digits.1, digit_concat);
        sum_calibration_vals += digit_concat;
    }
    println!("Total calibration sum = {}", sum_calibration_vals);
}

fn get_first_digit_in_str(input_string : &String) -> (i32, i32){
    for (idx, c) in input_string.chars().enumerate(){
        if let Some(ans) = c.to_digit(10){
            return (idx as i32, ans as i32);
        }
    }
    panic!("No digits found :(");
}

fn string_reverse(input_string : &String) -> String{
    return input_string.chars().rev().collect::<String>();
}

fn get_first_and_last_digits_in_str(input_string : &String) -> (i32, i32){
    let flipped_string = string_reverse(&input_string);

    let first_num_digit = get_first_digit_in_str(&input_string);
    let last_num_digit = get_first_digit_in_str(&flipped_string);

    let first_text_digit = find_first_number_substring(&input_string, false);
    let last_text_digit = find_first_number_substring(&flipped_string, true);

    let mut first_digit: i32 = 0;
    if first_num_digit.0 < first_text_digit.0{
        first_digit = first_num_digit.1;
    }
    else {
        first_digit = first_text_digit.1;
    }

    let mut last_digit: i32 = 0;
    if last_num_digit.0 < last_text_digit.0{
        last_digit = last_num_digit.1;
    }
    else {
        last_digit = last_text_digit.1;
    }
    return (first_digit, last_digit)
}

fn lines_from_file(filename: impl AsRef<Path>) -> Vec<String> {
    let file = File::open(filename).expect("no such file");
    let buf = BufReader::new(file);
    buf.lines()
        .map(|l| l.expect("Could not parse line"))
        .collect()
}

fn find_first_number_substring(input_string : &String, is_flipped : bool) -> (i32, i32){
    let mut digit_regex : Regex = Regex::new("(?:zero|one|two|three|four|five|six|seven|eight|nine)").unwrap();
    if is_flipped{
        digit_regex = Regex::new("(?:orez|eno|owt|eerht|ruof|evif|xis|neves|thgie|enin)").unwrap();
    }
    let matched_substr = digit_regex.find(input_string);
    match matched_substr{
        None => return (input_string.len() as i32, -1),
        Some(value) => {
            let mut num: i32 = 0;
            if !is_flipped{
                num = convert_text_digit_to_i32(&value.as_str());
            }
            else {
                num = convert_text_digit_to_i32(&value.as_str().chars().rev().collect::<String>());
            }
            let pos: i32 = value.start() as i32;
            //println!("Found a string number: {} which converts to: {} and was found at index: {}", value.as_str(), num, pos);
            let ans : i32 = num;
            return (pos,ans);
        }
    }
}

fn convert_text_digit_to_i32(input_str : &str) -> i32{
    match input_str {
        "zero" => return 0,
        "one" => return 1,
        "two" => return 2,
        "three" => return 3,
        "four" => return 4,
        "five" => return 5,
        "six" => return 6,
        "seven" => return 7,
        "eight" => return 8,
        "nine" => return 9,
        &_ => -1
    }
}