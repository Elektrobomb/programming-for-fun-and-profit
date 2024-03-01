cargo_build(){
  exit_code=0
  while read path; do
    printf "Project: %s\n" "$path"
    cargo build --verbose --manifest-path "$path" || exit_code=1
  done
  exit $exit_code
}

find . -name 'Cargo.toml' | sort -u | cargo_build