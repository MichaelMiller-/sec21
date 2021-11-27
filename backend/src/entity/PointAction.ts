import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, JoinColumn, OneToOne } from "typeorm";
import { LoadCase } from "./LoadCase";
import { StructuralPoint } from "./StructuralPoint";

@Entity()
export class PointAction {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @ManyToOne(type => StructuralPoint, pt => pt.id)
   referencePoint: StructuralPoint;

   @Column("real")
   value_x: number; // [kN]

   @Column("real")
   value_y: number; // [kN]

   @Column("real")
   value_z: number; // [kN]

   @OneToOne(type => LoadCase)
   @JoinColumn()
   loadcase: LoadCase;
}