import { MigrationInterface, QueryRunner } from "typeorm";

export class AddNameAndTypeToPointSupport1632732419401 implements MigrationInterface {
    name = 'AddNameAndTypeToPointSupport1632732419401'

    public async up(queryRunner: QueryRunner): Promise<void> {

        await queryRunner.query(`CREATE TYPE "point_support_type_enum" AS ENUM('fixed', 'hinged', 'sliding', 'custom')`);

        await queryRunner.query(`ALTER TABLE "point_support" ADD "name" character varying`);
        await queryRunner.query(`UPDATE "point_support" SET "name" = ''`);
        await queryRunner.query(`ALTER TABLE "point_support" ALTER COLUMN "name" SET NOT NULL`);

        await queryRunner.query(`ALTER TABLE "point_support" ADD "type" "point_support_type_enum"`);
        await queryRunner.query(`UPDATE "point_support" SET "type" = 'fixed'`);
        await queryRunner.query(`ALTER TABLE "point_support" ALTER COLUMN "type" SET NOT NULL`);
        await queryRunner.query(`ALTER TABLE "point_support" ALTER COLUMN "type" SET DEFAULT 'fixed'`);
    }

    public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`ALTER TABLE "point_support" DROP COLUMN "type"`);
        await queryRunner.query(`DROP TYPE "point_support_type_enum"`);
        await queryRunner.query(`ALTER TABLE "point_support" DROP COLUMN "name"`);
    }
}
