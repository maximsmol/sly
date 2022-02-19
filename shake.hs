import Development.Shake
import Development.Shake.Command
import Development.Shake.FilePath
import Development.Shake.Util

import Data.List
import System.Directory
import System.Environment
import Data.Maybe(fromMaybe)
import Control.Monad
import System.IO
import Control.Monad.State

dropDir :: FilePath -> FilePath -> FilePath
dropDir x base = joinPath $ dropDir' (splitPath x) (splitPath base)
  where
    dropDir' :: [FilePath] -> [FilePath] -> [FilePath]
    dropDir' xs [] = xs
    dropDir' (x:xs) (b:bs) =
      let diff = x \\ b
      in if diff == "" || diff == "/" then
          dropDir' xs bs
        else
          error $ "could not drop '"++(joinPath (b:bs))++"' from '"++(joinPath (x:xs))++"'"


shakeOptions' :: ShakeOptions
shakeOptions' = shakeOptions{
  shakeFiles = "bld"</>"shake"</>"db",
  shakeReport = (("bld"</>"rep") </>) <$> [
    "t"<.>"trace", "h"<.>"html"
  ],
  shakeLint = Just LintBasic,
  shakeTimings = True
}

main :: IO ()
main = shakeArgs shakeOptions' $ do
  let path_in = "src"
  let path_out = "bld"
  let path_dep = path_out</>"dep"

  let path_libin  = path_in </>"lib"
  let path_libout = path_out</>"lib"
  let path_libdep = path_dep</>"lib"

  let path_tstin  = path_in </>"tst"
  let path_tstout = path_out</>"tst"
  let path_tstdep = path_dep</>"tst"

  nixCFlags <- liftIO $ ((fromMaybe "") <$> lookupEnv "NIX_CFLAGS_COMPILE")
  nixLdFlags <- liftIO $ ((fromMaybe "") <$> lookupEnv "NIX_LDFLAGS")

  liftIO $ createDirectoryIfMissing True (path_out</>"rep")

  let exec = "dst"</>"dbg"</>"exec"
  -- let static_lib = "dst"</>"dbg"</>"sly.so" -- no support on macos
  let dynamic_lib = "dst"</>"dbg"</>"libsly.dylib"
  want [exec]

  dynamic_lib %> \out -> do
    let srcs = [
          path_libout</>"Animation"<.>"o",
          path_libout</>"Base"<.>"o",
          path_libout</>"Font"<.>"o",
          path_libout</>"Image"<.>"o",
          path_libout</>"Music"<.>"o",
          path_libout</>"Sound"<.>"o",
          path_libout</>"SpriteSheet"<.>"o"
          ]
    need srcs

    let libFlags = (("-l"++) <$> ["sdl2", "sdl2_image", "sdl2_ttf", "sdl2_mixer"]) ++ (words nixLdFlags)

    () <- cmd "clang++" "-shared" "-install_name" "@rpath/libsly.dylib" "-O0" "-o" [out] libFlags "-L/usr/local/opt/llvm/lib" srcs
    return ()

  exec %> \out -> do
    let srcs = [
          path_tstout</>"main"<.>"o"
          ]
    need srcs
    need [dynamic_lib]

    let libFlags = (("-l"++) <$> ["sdl2", "sly", "sdl2_image", "sdl2_ttf", "sdl2_mixer"]) ++ (words nixLdFlags)

    () <- cmd "clang++" "-rpath" "@executable_path" "-O0" "-o" [out] libFlags ("-L"++takeDirectory dynamic_lib) "-L/usr/local/opt/llvm/lib" srcs
    return ()

  [path_libout<//>"*"<.>"o", path_libdep<//>"*"<.>"dep"] &%> \[out, dep] -> do
    let src = path_libin</>(dropDir out path_libout)-<.>"cpp"
    --  "-Ofast"
    let diagFlags = ["-fcolor-diagnostics"]
    let warnFlags = ["-Weverything", "-Wno-c++98-compat", "-Wno-c++98-c++11-compat", "-Wno-c++98-c++11-compat-pedantic", "-Wno-c99-extensions", "-Wno-c++98-c++11-c++14-compat", "-Wno-padded"]
    let outputFlags = ["-o", out]
    let includeFlags = ("-isystem"++) <$> ["/usr/local/include/SDL2", "/usr/local/opt/llvm/include"]
    let otherFlags = ["-std=c++1z"]
    let command = ["clang++", "-O0"] ++ diagFlags ++ warnFlags ++ outputFlags ++ includeFlags ++ otherFlags ++ (words nixCFlags)

    () <- cmd command "-M" "-MF" [dep] [src]
    needMakefileDependencies dep

    () <- cmd command "-c" [src]
    return ()

  [path_tstout<//>"*"<.>"o", path_tstdep<//>"*"<.>"dep"] &%> \[out, dep] -> do
    let src = path_tstin</>(dropDir out path_tstout)-<.>"cpp"
    --  "-Ofast"
    let diagFlags = ["-fcolor-diagnostics"]
    let warnFlags = ["-Weverything", "-Wno-c++98-compat", "-Wno-c++98-c++11-compat", "-Wno-c++98-c++11-compat-pedantic", "-Wno-c99-extensions", "-Wno-c++98-c++11-c++14-compat", "-Wno-padded"]
    let outputFlags = ["-o", out]
    let includeFlags = ("-isystem"++) <$> ["/usr/local/include/SDL2", "/usr/local/opt/llvm/include"]
    let otherFlags = ["-std=c++1z"]
    let command = ["clang++", "-O0"] ++ diagFlags ++ warnFlags ++ outputFlags ++ includeFlags ++ otherFlags ++ (words nixCFlags)

    () <- cmd command "-M" "-MF" [dep] [src]
    needMakefileDependencies dep

    () <- cmd command "-c" [src]
    return ()

  phony "dbg" $ do
    return ()
