/*
 *   directory function tests 
 */

#include <tads.h>
#include <file.h>

cache: object
    dirtest = static new FileName('dirtest')
;

main()
{
    local ops = [
        'mkdir dirtest',
        'mkdir dirtest/one',
        'mkdir dirtest/two',
        'mkdir dirtest/three',
        'cp ../data/dirtest-file-a.txt dirtest/one/file-a.txt',
        'cp ../data/dirtest-file-b.txt dirtest/one/file-b.txt',
        'cp ../data/dirtest-file-c.txt dirtest/one/file-c.txt',
        'cp ../data/dirtest-file-a.txt dirtest/two/file-a.txt',
        'mv dirtest/two/file-a.txt dirtest/two/file-x.txt',
        'mv dirtest/two/file-x.txt dirtest/one/file-x.txt',
        'mv dirtest/one/file-x.txt dirtest/one/file-a.txt',
        'mv dirtest/one/file-x.txt dirtest/two/file-a.txt',
        'rmdir dirtest/one',
        'rmdir-r dirtest/one',
        'rmdir dirtest/two',
        'rm dirtest/two/file-a.txt',
        'rmdir dirtest/two',
        'rmdir dirtest',
        'rmdir-r dirtest'
    ];

    for (local op in ops)
        dirop(op);
}

dirop(op)
{
    "-----\n<<op>>: ";
    op = op.split(' ');
    local cmd = op[1];
    op = op.sublist(2).mapAll({x: FileName.fromUniversal(x)});
    try
    {
        switch (cmd)
        {
        case 'mkdir':
            op[1].createDirectory();
            break;

        case 'cp':
            copyfile(op[1], op[2]);
            break;

        case 'rmdir':
            op[1].removeDirectory();
            break;

        case 'rmdir-r':
            op[1].removeDirectory(true);
            break;

        case 'rm':
            op[1].deleteFile();
            break;

        case 'mv':
            op[1].renameFile(op[2]);
            break;
        }

        "ok\n";

        if (cache.dirtest.getFileType() != nil)
            ls(cache.dirtest);
    }
    catch (Exception exc)
    {
        "*** Exception: <<exc.displayException()>>\n";
    }
}

copyfile(src, dst)
{
    local fsrc = File.openRawFile(src, FileAccessRead);
    local fdst = File.openRawFile(dst, FileAccessWrite);

    local b;
    while ((b = fsrc.unpackBytes('b1024*')[1]).length() != 0)
        fdst.packBytes('b*', b);

    fsrc.closeFile();
    fdst.closeFile();

    return true;
}

ls(dir, level=0)
{
    for (local f in dir.listDir().sort(
        SortAsc, {a, b: a.getBaseName().compareIgnoreCase(b.getBaseName())}))
    {
        local info = f.getFileInfo();
        "<<makeString('\t', level)>><<%_\ -16s
          f.getBaseName().htmlify() + (info.isDir ? '/' : '')>>";
        if (!info.isDir)
            "<<%,d info.fileSize>> bytes";
        "\n";
        if (info.isDir && !info.specialLink)
            ls(f, level + 1);
    }
}

