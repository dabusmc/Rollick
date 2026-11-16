import os
from pathlib import Path

SOURCES_FILE = os.path.join(os.getcwd(), "python/sources.txt")

def readLines(path):
    lines = []
    with open(path, "r") as f:
        for line in f:
            if "\n" in line:
                lines.append(line.split("\n")[0])
            else:
                lines.append(line)
    return lines

def loadSources():
    return readLines(SOURCES_FILE)

def getParentDirectory(dir):
    path = Path(dir)
    return path.parent.absolute()

def getIncludePath(line):
    if '"' in line:
        return Path(line.split('"')[1])

    return None

def getFilesInDir(dir):
    present_files = []

    for subdir, dirs, files in os.walk(dir):
        for file in files:
            if Path(file).suffix in {".h", ".hpp", ".cpp", ".c"}:
                present_files.append(os.path.join(subdir, file))

    return present_files



def classifyInclude(line):
    if line.startswith("#include <"):
        include = line.split("<")[1].split(">")[0]
        return ("external", Path(include))

    include = line.split('"')[1]
    return ("internal", Path(include))

def getCorrespondingHeader(source, module_root):
    source = Path(source)
    module_root = Path(module_root)

    extensions = {
        ".cpp": [".h", ".hpp"],
        ".c": [".h"]
    }

    for extension in extensions.get(source.suffix, []):
        header_name = source.stem + extension
        matches = list(module_root.rglob(header_name))

        if len(matches) == 1:
            return matches[0].relative_to(module_root)

    return None

def getExpectedIncludes(headers, source, module_root):
    corresponding = getCorrespondingHeader(source, module_root)

    first = []
    internal = []
    external = []

    for header in headers:
        category, include = classifyInclude(header)

        if category == "external":
            external.append(header)
        elif corresponding and include == corresponding:
            first.append(header)
        else:
            internal.append(header)

    internal.sort(key=lambda line: (
        -len(Path(line.split('"')[1]).parent.parts),
        str(Path(line.split('"')[1]).parent).lower(),
        Path(line.split('"')[1]).name.lower()
    ))

    external.sort(key=lambda line: line.split("<")[1].split(">")[0].lower())

    result = first + internal + external
    return result

def getIncludeBlocks(lines):
    blocks = []
    current_block = []

    for line in lines:
        if line.strip().startswith("#include"):
            current_block.append(line.strip())
        elif not line.strip():
            if current_block:
                blocks.append(current_block)
                current_block = []
        elif current_block:
            blocks.append(current_block)
            current_block = []

    if current_block:
        blocks.append(current_block)

    return blocks

def checkIncludeBlocks(blocks):
    for block in blocks:
        categories = [classifyInclude(line)[0] for line in block]

        if "external" in categories:
            if any(category != "external" for category in categories):
                return False, f"Mixed internal and external includes: {block}"
            continue

        directories = [classifyInclude(line)[1].parts[0] for line in block]
        if len(set(directories)) > 1:
            return False, f"Includes from different top-level directories: {block}"

        common = list(directories[0])
        for directory in directories[1:]:
            while common and tuple(common) != tuple(directory[:len(common)]):
                common.pop()

        if not common:
            return False, f"Includes from unrelated directories: {block}"

    return True, None

def checkFile(path, module_root):
    print(path)

    lines = readLines(path)
    headers = [
        line.strip()
        for line in lines
        if line.strip().startswith("#include")
    ]

    expected = getExpectedIncludes(headers, path, module_root)
    corresponding = getCorrespondingHeader(path, module_root)

    if headers != expected:
        print("Incorrect include ordering.")
        print(f"Corresponding header: {corresponding}")

        print("\nActual:")
        for header in headers:
            print(f"  {header}")

        print("\nExpected:")
        for header in expected:
            print(f"  {header}")

    blocks = getIncludeBlocks(lines)
    valid_blocks, error = checkIncludeBlocks(blocks)

    if not valid_blocks:
        print(f"Incorrect include block: {error}")

    print("Okay" if headers == expected and valid_blocks else "Wrong")

def main():
    root_dir = getParentDirectory(os.getcwd())
    sources = loadSources()

    for source in sources:
        print(f"\nChecking {source} directory.")
        source_dir = os.path.join(root_dir, source)

        all_files = getFilesInDir(source_dir)
        for file in all_files:
            checkFile(file, source_dir)

if __name__ == "__main__":
    main()