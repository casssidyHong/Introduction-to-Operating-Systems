#!/bin/bash

print_path=false
path="" # Declare path as a global variable

# Read parent pid, child pid
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      print_path=true
      shift
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi

# Function to check if a process is the ancestor of another
is_ancestor() {
  local parent_pid="$1"
  local child_pid="$2"
  while [ "$child_pid" -ne 0 ]; do
    if [ "$child_pid" -eq "$parent_pid" ]; then
      return 0 # The parent is an ancestor of the child
    fi
    path="$child_pid -> $path" # Append intermediate PID to the path
    child_pid=$(cat "/proc/$child_pid/status" 2>/dev/null | grep PPid | awk '{print $2}')
  done
  return 1 # The parent is not an ancestor of the child
}

# Check if the parent is an ancestor of the child
if is_ancestor "$parent" "$child"; then
  echo "Yes"
  if [ "$print_path" = true ]; then
    path="$parent -> $path"
    path="${path%->*}"
    echo "$path"
  fi
else
  echo "No"
fi
