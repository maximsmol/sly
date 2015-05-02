env = Environment(CXX = 'clang++')

env.Append(CXXFLAGS = '--std=c++11 -stdlib=libc++')
env.Append(LINKFLAGS = '--stdlib=libc++')

env.Append(CXXFLAGS = '-Weverything')
env.Append(CXXFLAGS = '-Wno-c++98-compat')


SDL_LIBS = ['SDL2', 'SDL2_image', 'SDL2_ttf', 'SDL2_mixer']
env.Append(CXXFLAGS = '-isystem /usr/local/include/SDL2')


slyFiles = env.Glob('src/sly/*.cpp')
sly = env.SharedLibrary('lib/sly', slyFiles, LIBS=SDL_LIBS)

testFiles = env.Glob('src/*.cpp')
test = env.Program('bin/test', testFiles, CPPPATH=['include'], LIBS=SDL_LIBS+['sly'], LIBPATH='lib')
env.Depends(test, sly)
