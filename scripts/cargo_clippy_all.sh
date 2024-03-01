cargo_clippy(){
  exit_code=0
  while read path; do
    printf "Project: %s\n" "$path"
    path_without_file="${path%"Cargo.toml"}"
    cd "$path_without_file"
    cargo clippy --verbose || exit_code=1
    cd -
  done
  exit $exit_code
}

find . -name 'Cargo.toml' | sort -u | cargo_clippy