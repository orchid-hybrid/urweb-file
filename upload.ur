fun echo r =
  if blobSize (fileData r.File) > 100000 then
      return <xml>Whoa!  That one's too big.</xml>
  else
      b <- File.save "/home/urweb/urweb-file/file.txt" r.File;
      if b then
        return <xml>Managed to save the file.</xml>
      else
        return <xml>Failed to save the file.</xml>

fun main () = return <xml><body>
  <h1>The Amazing File Saver!</h1>

  <form>Upload a file: <upload{#File}/> <submit action={echo}/></form>
</body></xml>

