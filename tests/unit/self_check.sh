#!/usr/bin/env bash
set -eEuo pipefail
trap 'echo "[ERROR] ${BASH_SOURCE[0]}:${LINENO}: \"${BASH_COMMAND}\" failed" >&2' ERR

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/../../scripts/lib/config.sh"
# shellcheck disable=SC1091
source "$LIB_DIR/logging.sh"

BINARY="${1:-$BIN_DIR/bulk}"

check_output() {
    local name="$1"
    local expected="$2"
    local actual="$3"

    if [[ "$actual" != "$expected" ]]; then
        log_error "Test failed: $name"
        log_error "Expected: [$expected]"
        log_error "Actual:   [$actual]"
        exit 1
    fi

    log_ok "Passed: $name"
}

run_case() {
    local name="$1"
    local input="$2"
    local expected="$3"
    local workdir
    local output

    workdir="$(mktemp -d)"
    output="$(cd "$workdir" && printf '%s' "$input" | "$BINARY" 3)"
    rm -rf "$workdir"

    check_output "$name" "$expected" "$output"
}

run_error_case() {
    local name="$1"
    local expected="$2"
    local output

    if output="$("$BINARY" 2>&1)"; then
        log_error "Test failed: $name"
        log_error "Expected non-zero exit code"
        exit 1
    fi

    check_output "$name" "$expected" "$output"
}

main() {
    if [[ ! -x "$BINARY" ]]; then
        log_error "Binary not found or not executable: $BINARY"
        exit 1
    fi

    run_case \
        "static blocks and eof flush" \
        $'cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n' \
        $'bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5'

    run_case \
        "dynamic blocks and ignored unfinished block" \
        $'cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}\n{\ncmd10\ncmd11\n' \
        $'bulk: cmd1, cmd2\nbulk: cmd3, cmd4\nbulk: cmd5, cmd6, cmd7, cmd8, cmd9'

    run_error_case \
        "missing block size argument" \
        "Expected one argument: block size"

    log_ok "Self-check passed"
}

main "$@"
