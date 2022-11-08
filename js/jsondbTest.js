import { JsonDB } from './jsondb.js'

const text = await Deno.readTextFile("./lines.txt")
let lines = text.split(/\r?\n/).filter((x)=>x.length>0)
console.log('lines=', lines)
let objs = lines.map((line, i)=>{return {id:i, quote:line}})
console.log('objs=', objs)

let db = new JsonDB()
let docs = db.load(objs)
console.log('match(unix)=', docs.match('unix'))
