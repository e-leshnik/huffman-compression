import csv
import statistics
import subprocess
import time
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
BINARY = PROJECT_ROOT / "build" / "huffman_archiver"
INPUT_ROOT = PROJECT_ROOT / "experiments" / "input"
OUTPUT_ROOT = PROJECT_ROOT / "experiments" / "output"
CSV_FILE = PROJECT_ROOT / "experiments" / "results_all_multi_run.csv"

RUNS = 30

OUTPUT_ROOT.mkdir(parents=True, exist_ok=True)

CATEGORIES = {
    "text": ["*.txt"],
    "code": ["*.txt"],
    "compressed": ["*.jpg", "*.jpeg", "*.png", "*.zip", "*.bin"],
}

def run_command(cmd):
    start = time.perf_counter()
    result = subprocess.run(cmd, capture_output=True, text=True)
    end = time.perf_counter()
    return result, end - start

def compression_ratio(original_size, compressed_size):
    return compressed_size / original_size if original_size != 0 else 0.0

def relative_std_percent(mean_value, std_value):
    return (std_value / mean_value * 100) if mean_value != 0 else 0.0

def files_equal(file1, file2):
    return file1.read_bytes() == file2.read_bytes()

def mean_std(values):
    if len(values) == 1:
        return values[0], 0.0
    return statistics.mean(values), statistics.stdev(values)

def collect_input_files(input_dir, patterns):
    files = []
    for pattern in patterns:
        files.extend(input_dir.glob(pattern))
    return sorted(files)

def collect_category(category_name, patterns, rows):
    input_dir = INPUT_ROOT / category_name
    output_dir = OUTPUT_ROOT / category_name
    output_dir.mkdir(parents=True, exist_ok=True)

    input_files = collect_input_files(input_dir, patterns)

    for input_file in input_files:
        compressed_file = output_dir / f"{input_file.stem}.bin"
        restored_file = output_dir / f"{input_file.stem}_out{input_file.suffix}"

        compress_cmd = [
            str(BINARY),
            "compress",
            str(input_file),
            str(compressed_file),
        ]

        decompress_cmd = [
            str(BINARY),
            "decompress",
            str(compressed_file),
            str(restored_file),
        ]

        compress_result, _ = run_command(compress_cmd)
        if compress_result.returncode != 0:
            raise RuntimeError(
                f"Compression failed for {input_file.name}:\n{compress_result.stderr}"
            )

        decompress_result, _ = run_command(decompress_cmd)
        if decompress_result.returncode != 0:
            raise RuntimeError(
                f"Decompression failed for {input_file.name}:\n{decompress_result.stderr}"
            )

        original_size = input_file.stat().st_size
        compressed_size = compressed_file.stat().st_size
        restored_size = restored_file.stat().st_size
        correct = files_equal(input_file, restored_file)

        compress_times = []
        for _ in range(RUNS):
            result, elapsed = run_command(compress_cmd)
            if result.returncode != 0:
                raise RuntimeError(
                    f"Compression failed during repeated runs for {input_file.name}:\n{result.stderr}"
                )
            compress_times.append(elapsed)

        decompress_times = []
        for _ in range(RUNS):
            result, elapsed = run_command(decompress_cmd)
            if result.returncode != 0:
                raise RuntimeError(
                    f"Decompression failed during repeated runs for {input_file.name}:\n{result.stderr}"
                )
            decompress_times.append(elapsed)

        compress_mean, compress_std = mean_std(compress_times)
        decompress_mean, decompress_std = mean_std(decompress_times)

        compress_std_percent = relative_std_percent(compress_mean, compress_std)
        decompress_std_percent = relative_std_percent(decompress_mean, decompress_std)

        rows.append({
            "category": category_name,
            "file": input_file.name,
            "original_size_bytes": original_size,
            "original_size_kb": round(original_size / 1024, 1),
            "compressed_size_bytes": compressed_size,
            "restored_size_bytes": restored_size,
            "compression_ratio": round(compression_ratio(original_size, compressed_size), 6),
            "compress_mean_sec": round(compress_mean, 6),
            "compress_std_sec": round(compress_std, 6),
            "compress_std_percent": round(compress_std_percent, 2),
            "decompress_mean_sec": round(decompress_mean, 6),
            "decompress_std_sec": round(decompress_std, 6),
            "decompress_std_percent": round(decompress_std_percent, 2),
            "runs": RUNS,
            "correct": correct,
        })

def main():
    rows = []

    for category_name, patterns in CATEGORIES.items():
        collect_category(category_name, patterns, rows)

    with open(CSV_FILE, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(
            f,
            fieldnames=[
                "category",
                "file",
                "original_size_bytes",
                "original_size_kb",
                "compressed_size_bytes",
                "restored_size_bytes",
                "compression_ratio",
                "compress_mean_sec",
                "compress_std_sec",
                "compress_std_percent",
                "decompress_mean_sec",
                "decompress_std_sec",
                "decompress_std_percent",
                "runs",
                "correct",
            ],
        )
        writer.writeheader()
        writer.writerows(rows)

    print(f"Done. Results saved to: {CSV_FILE}")

if __name__ == "__main__":
    main()