#!/usr/bin/env python3

"""
Creates a fallback SQLite dictionary database for weighted word distance.
"""

import argparse
import re
import sqlite3


def parse_args():
    parser = argparse.ArgumentParser(
        description="Create a words SQLite database from a plain text word list"
    )
    parser.add_argument("word_list", help="Path to a newline-delimited word list")
    parser.add_argument("output_db", help="Output SQLite database path")
    return parser.parse_args()


def normalize(word):
    return re.sub(r"[^\w]", "", word.lower())


def main():
    args = parse_args()

    conn = sqlite3.connect(args.output_db)
    cur = conn.cursor()
    cur.execute("DROP TABLE IF EXISTS words")
    cur.execute("CREATE TABLE words (word TEXT NOT NULL UNIQUE, count INTEGER NOT NULL)")

    with open(args.word_list, "r", encoding="utf-8", errors="ignore") as source:
        for line in source:
            word = normalize(line.strip())
            if word:
                cur.execute(
                    "INSERT OR IGNORE INTO words(word, count) VALUES (?, 1)", (word,)
                )

    conn.commit()
    conn.close()


if __name__ == "__main__":
    main()
