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
		let docs = this.docs.filter(f)
		return new Docs(docs)
	}

	where(f) {
		return this.filter((x)=>f(x.obj))
	}

	sort(key, order="INC") {
		let docs = this.docs.sort((x,y)=>x.obj[key]<y.obj[key])
		if (order=="DESC") docs = docs.reverse()
		return new Docs(docs)
	}
	
	match(q) {
		let q0 = q.toLowerCase()
		return this.filter((doc)=>doc.text.indexOf(q0)>=0)
	}

	toObj() {
		return this.docs.map((x)=>x['obj'])
	}

	toText() {
		return this.docs.map((x)=>x['text'])
	}

	toString() {
		return this.toText().join('\n')
	}
}

export class JsonDB {
	load(array) {
		return new Docs(array)
	}
}
