// todo: generate from backend TypeORM entities

export interface DbProject {
   id: number;
   name: string;
   title: string;
   createdAt: Date;
   modifiedAt: Date;
}

export interface DbStructuralPoint {
   id: number;
   name: string;
   uuid: string;
   coordinate_x: number;
   coordinate_y: number;
   coordinate_z: number;
}

export interface DbMaterial {
   id: number;
   name: string;
   uuid: string;
   E_modulus: number;
}

export interface DbCrossSection {
   id: number;
   name: string;
   uuid: string;
//   material: DbMaterial;
   area: number;
}

export interface DbCurveMember {
   id: number;
   name: string;
   uuid: string;
   systemLine: string;
   beginNode: DbStructuralPoint;
   endNode: DbStructuralPoint;
}

export interface DbLoadCase {
   id: number;
   name: string;
   uuid: string;
   description: string;
   actionType: string;
   type: string;
}
