import { JsonDB } from './jsondb.js'

const text = await Deno.readTextFile("./lines.txt")
let lines = text.split(/\r?\n/).filter((x)=>x.length>0)
console.log('lines=', lines)
let objs = lines.map((line, i)=>{return {id:i, quote:line}})
console.log('objs=', objs)

let db = new JsonDB()
var docs
docs = await db.loadCsvFile('test.csv')
console.log('docs=', docs)
console.log('match(ccc)=', docs.match('ccc'))

docs = db.loadObjs(objs)
console.log('match(費曼).match(科學).sort(id, DESC)=', docs.match('費曼').match('科學').sort('id', 'DESC'))


