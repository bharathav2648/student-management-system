#!/usr/bin/env bash

# file-management.sh
# A simple Linux shell script to automate common file management tasks.

set -euo pipefail
IFS=$'\n\t'

print_usage() {
  cat <<EOF
Usage: ${0##*/} [OPTIONS] [TARGET_DIR]

Options:
  -b, --backup        Create a dated backup archive of TARGET_DIR.
  -o, --organize      Organize files in TARGET_DIR by extension into folders.
  -c, --clean         Remove temporary and old files from TARGET_DIR.
  -a, --all           Run backup, organize, and clean in sequence.
  -n, --days DAYS     Use DAYS when cleaning old files (default: 30).
  -l, --log [FILE]    Write activity to FILE (default: file-management.log).
  -h, --help          Show this help message.

If TARGET_DIR is omitted, the current directory is used.
EOF
}

TARGET_DIR="."
ACTION=""
DAYS=30
LOG_FILE="file-management.log"
LOG_ENABLED=false

while [[ $# -gt 0 ]]; do
  case "$1" in
    -b|--backup)
      ACTION="backup"
      shift
      ;;
    -o|--organize)
      ACTION="organize"
      shift
      ;;
    -c|--clean)
      ACTION="clean"
      shift
      ;;
    -a|--all)
      ACTION="all"
      shift
      ;;
    -n|--days)
      DAYS="${2:-}"
      if [[ -z "$DAYS" ]]; then
        echo "Error: --days requires a numeric value." >&2
        exit 1
      fi
      shift 2
      ;;
    -l|--log)
      LOG_ENABLED=true
      if [[ -n "${2:-}" && ! "${2:-}" =~ ^- ]]; then
        LOG_FILE="$2"
        shift 2
      else
        shift
      fi
      ;;
    -h|--help)
      print_usage
      exit 0
      ;;
    *)
      TARGET_DIR="$1"
      shift
      ;;
  esac
done

if [[ ! -d "$TARGET_DIR" ]]; then
  echo "Error: target directory '$TARGET_DIR' does not exist." >&2
  exit 1
fi

cd "$TARGET_DIR"

log_action() {
  local message="$1"
  echo "$message"
  if [[ "$LOG_ENABLED" == true ]]; then
    printf '%s %s\n' "$(date +'%Y-%m-%d %H:%M:%S')" "$message" >> "$LOG_FILE"
  fi
}

backup() {
  local timestamp backup_name
  timestamp=$(date +"%Y%m%d-%H%M%S")
  backup_name="backup-${timestamp}.tar.gz"
  log_action "Creating backup archive: $backup_name"
  tar -czf "$backup_name" .
  log_action "Backup completed."
}

organize() {
  log_action "Organizing files by extension..."
  find . -maxdepth 1 -type f | while read -r file; do
    filename=$(basename "$file")
    extension="${filename##*.}"

    if [[ "$filename" == "$extension" ]] || [[ -z "$extension" ]]; then
      folder="misc"
    else
      folder="${extension,,}"
    fi

    mkdir -p "$folder"
    mv -n "$file" "$folder/"
  done
  log_action "Organization complete."
}

clean() {
  log_action "Cleaning temporary and old files older than $DAYS days..."
  find . -type f \( -name '*.tmp' -o -name '*.bak' -o -name '*.swp' -o -name '*~' -o -name '*.log' \) -mtime +"$DAYS" -print -delete
  log_action "Clean complete."
}

case "$ACTION" in
  backup)
    backup
    ;;
  organize)
    organize
    ;;
  clean)
    clean
    ;;
  all)
    backup
    organize
    clean
    ;;
  *)
    echo "Error: No action selected. Use --help for usage." >&2
    exit 1
    ;;
esac
