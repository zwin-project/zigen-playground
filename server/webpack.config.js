const path = require('path')
const DevPlugin = require('./DevPlugin.js')

module.exports = (env, argv) => ({
  entry: './front/index.tsx',

  mode: argv.mode || 'development',

  output: {
    filename: 'bundle.js',
    path: path.resolve(__dirname, 'public/js/'),
  },

  resolve: {
    extensions: ['.tsx', '.ts', '.js', 'css'],
    // alias: {
    //   lib: path.resolve(__dirname, 'src/lib'),
    //   config: path.resolve(__dirname, 'src/config'),
    // },
  },

  devtool: 'source-map',

  module: {
    rules: [
      {
        test: /\.tsx?$/,
        exclude: /node_modules/,
        use:
          argv.mode === 'development'
            ? ['ts-loader']
            : ['babel-loader', 'ts-loader'],
      },
    ],
  },
  plugins: [new DevPlugin()],
})
