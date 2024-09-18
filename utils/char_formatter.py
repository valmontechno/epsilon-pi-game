from PIL import Image
from tkinter import filedialog, simpledialog;

def main():
    path = filedialog.askopenfilename()
    if path == '': return

    img = Image.open(path)
    img = img.convert('L')
    width, height = img.size

    result = []

    for i in range(height):
        for j in range(width):
            result.append(str(255 - img.getpixel((j, i))))

    print(f'{width}x{height} [{width*height}]')
    print('{' + ','.join(result) + '}')
    input()

main()
