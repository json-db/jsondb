export async function fileToLines(file) {
	const text = await Deno.readTextFile(file)
	let lines = text.split(/\r?\n/)
	                .filter((line)=>line.length>0)
	return lines
}

export function csvToObj(line, fields) {
	fields = fields || this.fields
	let values = line.split(",")
	let obj = {}
	for (let i=0; i<fields.length; i++) {
		obj[fields[i]] = values[i]
	}
	return obj
} 

export class Docs {
	constructor(array) {
		this.docs = []
		for (let x of array) {
			let doc
			if (typeof x == "string")
				doc = {obj:JSON.parse(x), text:x.toLowerCase()}
			else
				doc = {obj:x, text:JSON.stringify(x).toLowerCase()}
			this.docs.push(doc)
		}
	}

	filter(f) {
		let r = new Docs([])
		r.docs = this.docs.filter(f)
		return r
	}

	where(f) {
		return this.filter((x)=>f(x.obj))
	}

	sort(key, order="INC") {
		let docs = this.docs.sort((x,y)=>x.obj[key]<y.obj[key])
		if (order=="DESC") docs = docs.reverse()
		let r = new Docs([])
		r.docs = docs
		return r
	}
	
	match(q) {
		let q0 = q.toLowerCase()
		return this.filter((doc)=>doc.text.indexOf(q0)>=0)
	}

	toObj() {
		return this.docs.map((x)=>x.obj)
	}

	toText() {
		return this.docs.map((x)=>x.text)
	}

	toString() {
		return this.toText().join('\n')
	}
}

export class JsonDB {
	constructor() {
	}
	connectDb(url, dbType) {
		this.dbType = dbType // type: docDB, sqlite, postgres
		this.url = url
	}
	loadObjs(objs) {
		return new Docs(objs)
	}
	loadJsons(jsons) {
		return new Docs(jsons)
	}
	loadCsv(lines, fields) {
		let docs = []
		fields = fields || this.fields
		for (let line of lines) {
			let obj = csvToObj(line, fields)
			console.log('csv:obj=', obj)
			docs.push(obj)
		}
		return new Docs(docs)
	}
	async loadCsvFile(file) {
		let lines = await fileToLines(file)
		let fields = lines[0].split(",")
		return this.loadCsv(lines.slice(1), fields)
	}
	async loadJsonFile(file) {
		let lines = await fileToLines(file)
		return this.loadJsons(lines, fields)
	}
}
