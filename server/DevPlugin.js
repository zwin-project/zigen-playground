const pluginName = 'DevPlugin'

const path = require('path')
const filecopy = require('filecopy')

const DevPlugin = function () {}

DevPlugin.prototype.apply = function (compiler) {
  compiler.hooks.compile.tap(pluginName, (pram) => {
    console.log('\x1B[2J')
    console.clear()
  })
  compiler.hooks.done.tap(pluginName, (stats) => {
    if (stats.compilation.errors[0]) {
      // transpileがエラーを吐くとき
      // do nothing
    } else {
      // 成功したとき
      buildSuccessDone()
    }

    console.log(
      '\n\x1B[36m' + 'End at ' + new Date().toLocaleTimeString() + '\x1B[39m'
    )
  })
}

function buildSuccessDone() {
  // copy static files
  const beforePath = path.join(__dirname, 'front/static/*')
  const afterPath = path.join(__dirname, 'public/')
  filecopy(beforePath, afterPath, {}).then(() => {
    staticFilesCopyDone(beforePath, afterPath)
  })
}

function staticFilesCopyDone(beforePath, afterPath) {
  console.log('\n*--- FROM ' + pluginName + ' ---*')
  console.log('\ncopied static files.')
  console.log('\n from ' + beforePath)
  console.log('\n  to  ' + afterPath)

  // more
}

module.exports = DevPlugin
