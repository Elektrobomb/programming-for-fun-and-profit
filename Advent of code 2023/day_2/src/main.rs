use std::{
    fs::File,
    io::{prelude::*, BufReader},
    path::Path, string, clone,
};
use regex::{Regex, Match};

const MAX_RED : i32 = 12;
const MAX_GREEN : i32 = 13;
const MAX_BLUE : i32 = 14;

fn main() {
    let data = lines_from_file("./Advent of code 2023/day_2/data/input.txt");
    
    let mut sum_valid_games : i32 = 0;
    let mut sum_game_power : i32 = 0;
    for line in data{
        if is_game_valid(&line){
            sum_valid_games += parse_game_number(&line);
        }
        sum_game_power += calculate_game_power(&line);
    }
    println!("Sum game numbers: {}", sum_valid_games);
    println!("Sum game powers: {}", sum_game_power);
}

fn lines_from_file(filename: impl AsRef<Path>) -> Vec<String> {
    let file = File::open(filename).expect("no such file");
    let buf = BufReader::new(file);
    buf.lines()
        .map(|l| l.expect("Could not parse line"))
        .collect()
}

fn separate_string(input_string : &String, delimeter : char) -> Vec<String> {
    let parts: Vec<String> = input_string.split(delimeter).map(str::to_string).collect();
    return parts;
}

fn parse_game_number(input_string : &String) -> i32{
    let game_part: &String = &separate_string(input_string, ':')[0];
    let number_part: String = String::from(&game_part[5..]);
    let game_number: i32 = number_part.parse().unwrap();
    return game_number;
}

fn parse_cube_numbers_in_run(run_string : &String) -> (i32, i32, i32){
    let mut count_red : i32 = 0;
    let mut count_blue : i32 = 0;
    let mut count_green : i32 = 0;

    let picked_colours: Vec<String> = separate_string(run_string, ',');
    for picked_colour in &picked_colours{
        let split_picked_colour = separate_string(picked_colour, ' ');

        match split_picked_colour[2].as_str() {
            "red" => count_red += split_picked_colour[1].parse::<i32>().unwrap(),
            "green" => count_green += split_picked_colour[1].parse::<i32>().unwrap(),
            "blue" => count_blue += split_picked_colour[1].parse::<i32>().unwrap(),
            &_ => panic!("{} is not a valid colour!!!", split_picked_colour[2].as_str())
        }

    }
    return (count_red, count_green, count_blue);
}

fn get_game_cube_values(game_string : &String) -> Vec<(i32, i32, i32)> {
    let content_part: &String = &separate_string(game_string, ':')[1];
    let runs: Vec<String> = separate_string(content_part, ';');
    
    let mut game_cube_values : Vec<(i32, i32, i32)> = Vec::new();
    
    for run in runs{
        game_cube_values.push(parse_cube_numbers_in_run(&run));
    }

    return game_cube_values;
}

fn is_game_valid(game_string : &String) -> bool {
    let game_cube_values = get_game_cube_values(game_string);

    for run_cube_values in game_cube_values{
        if run_cube_values.0 > MAX_RED{
            return false;
        }
        else if run_cube_values.1 > MAX_GREEN{
            return false;
        }
        else if run_cube_values.2 > MAX_BLUE{
            return false;
        }
    }
    return true;
}

fn find_max_cubes(game_string : &String) -> (i32, i32, i32) {
    let mut max_game_cubes : (i32,i32,i32) = (0,0,0);
    let game_cube_values = get_game_cube_values(game_string);

    for run_cube_values in game_cube_values{
        if run_cube_values.0 > max_game_cubes.0 {
            max_game_cubes.0 = run_cube_values.0;
        }
        if run_cube_values.1 > max_game_cubes.1 {
            max_game_cubes.1 = run_cube_values.1;
        }
        if run_cube_values.2 > max_game_cubes.2 {
            max_game_cubes.2 = run_cube_values.2;
        }
    }

    return max_game_cubes;
}

fn calculate_game_power(game_string : &String) -> i32 {
    let max_cubes = find_max_cubes(game_string);

    return max_cubes.0 * max_cubes.1 * max_cubes.2;
}